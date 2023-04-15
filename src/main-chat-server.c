#include "error.h"
#include "ncurses_ui.h"
#include "option_handler.h"
#include "server.h"
#include "util.h"
#include "my_ndbm.h"
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>


#define BACKLOG (5)
#define MAX_CLIENTS (255)
#define ONE_MILLISECOND (1000)


volatile bool server_running;


static void init_pollfd(struct pollfd* pollfds, int server_sock);
void* run_server_thread(void* arg);
int run_server(struct options* opts);
int setup_db_paths(void);


connected_user active_users[MAX_CLIENTS]; // active user cache w/ upper limit of 255 concurrent clients
char DB_LOGIN_INFO_PATH[PATH_MAX];
char DB_DISPLAY_NAMES_PATH[PATH_MAX];
char DB_USER_ACCOUNT_PATH[PATH_MAX];
char DB_CHANNEL_INFO_PATH[PATH_MAX];
char DB_MESSAGE_INFO_PATH[PATH_MAX];


int main(int argc, char *argv[])
{
    // initiate options, and parse command line argument
    struct options opts;
    options_init(&opts);
    if (parse_arguments(argc, argv, &opts) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);                                              // NOLINT(concurrency-mt-unsafe)
    }

    // setup DBs' path
    int result = setup_db_paths();
    if (result != EXIT_SUCCESS) {
        perror("setting up db paths");
        exit(EXIT_FAILURE);
    }

    init_ncurses();
    WINDOW* menu_win = create_menu_window();
    WINDOW* db_path_win = create_db_path_window();
    keypad(menu_win, TRUE);
    int choice;
    bool quit = false;
    server_running = false;
    pthread_t server_thread;

    int startx = (COLS - (int) strlen(TITLE)) / 2;

    // initialize active users
    for (int i = 0; i < MAX_CLIENTS; i++) {
        memset(&(active_users[i]), 0, sizeof(connected_user));
    }

    // Display menu
    while (!quit) {
        print_title(startx);
        print_db_paths(db_path_win, 2);
        choice = navigate_menu(menu_win);

        switch (choice) {
            case 1:
                if (!server_running) {
                    server_running = true;
                    pthread_create(&server_thread, NULL, run_server_thread, (void *) &opts);
                    while (server_running) {
                        int ch = getch();
                        if (ch == 'q') {
                            server_running = false;
                            clear();
                            refresh();
                        }
                        usleep(ONE_MILLISECOND); // Sleep for 1ms to avoid busy waiting
                    }
                    pthread_join(server_thread, NULL);
                }
                break;
            case 2:
                run_db_viewer_wrapper();
                break;
            case 3:
                run_active_user_viewer(active_users);
                break;
            case 4:
                quit = true;
                break;
            default:
                break;
        }

    }

    // Clean up
    clrtoeol();
    refresh();
    endwin();
    return 0;
}

static void init_pollfd(struct pollfd* pollfds, int server_sock)
{
    memset(pollfds, 0, sizeof(struct pollfd));
    pollfds[0].fd = server_sock;
    pollfds[0].events = POLLIN;
    pollfds[0].revents = 0;

    // initialize pollfds except the first one which is for listening
    for (int i = 1; i < MAX_CLIENTS; i++) {
        pollfds[i].fd = -1;
    }
}

void* run_server_thread(void* arg)
{
    clear();
    refresh();

    struct options* opts = (struct options*) arg;
    if (run_server(opts) == 0) {
        clear();
        refresh();
    } else {
        printw("Please press \'q\' to go back to the menu\n");
        refresh();
    }

    return NULL;
}

int run_server(struct options* opts)
{
    struct sockaddr_in client_addr;
    int option;
    char client_addrs[MAX_CLIENTS][MAX_IP_ADD_STR_LENGTH];
    memset(client_addrs, 0, MAX_CLIENTS * MAX_IP_ADD_STR_LENGTH);

    // Create a socket for the serve
    opts->server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (opts->server_sock < 0) {
        printw("[SERVER] failure to open the socket. try again\n");
        refresh();
        return -1;
    }
    // set the option
    if (setsockopt(opts->server_sock, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) < 0) {
        close(opts->server_sock);
        printw("[SERVER] failure to setup the socket. please try again later\n");
        refresh();
        return -1;
    }

    // Bind the socket to an address and port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(opts->port_in);
    if (bind(opts->server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        close(opts->server_sock);
        printw("[SERVER] the OS didn't release the port yet. please try again later\n");
        refresh();
        return -1;
    }

    // Listen for incoming connections
    if (listen(opts->server_sock, BACKLOG) < 0) {
        close(opts->server_sock);
        printw("[SERVER] failure to listen the socket. please try again later\n");
        refresh();
        return -1;
    }

    // Initialize the pollfd structure for the poll-server socket
    struct pollfd pollfds[MAX_CLIENTS];
    init_pollfd(pollfds, opts->server_sock);
//    fprintf(stdout, "Listening on port %d\n", opts->port_in);
    printw("Listening on port %d\n", opts->port_in);
    refresh();

    // Create admin account after successful server initialization
    create_admin();

    // Create global channel
    create_global_channel();

    while (server_running) {
        // Call poll() to wait for events on the file descriptors
        int num_fds = MAX_CLIENTS;
        int result = poll(pollfds, num_fds, 10); // -1 means infinite waiting
        if (result > 0) {
            // Check if there's a new client connection
            if (pollfds[0].revents == POLLIN) {
                memset(&client_addr, 0, sizeof(struct sockaddr_in));
                socklen_t socket_len = sizeof(client_addr);
                int client_sock = accept(opts->server_sock, (struct sockaddr *) &client_addr, &socket_len);
                if (client_sock < 0) {
                    close(opts->server_sock);
                    fatal_errno(__FILE__, __func__ , __LINE__, errno, 2);
                }

                // Add the new client socket to the pollfd structure
                int i;
                for (i = 1; i < MAX_CLIENTS; i++) {
                    if (pollfds[i].fd == -1) {
                        pollfds[i].fd = client_sock;
                        pollfds[i].events = POLLIN;
                        pollfds[i].revents = 0;
                        get_ip_str((const struct sockaddr*) &client_addr, client_addrs[i]);
                        break;
                    }
                }
            }

            // Check if there's data to be read from the client sockets
            for (int i = 1; i < MAX_CLIENTS; i++) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
                switch (pollfds[i].revents) {
                    // no events
                    case 0:
                        break;

                    case POLLIN:
                        // read request (data is ready)
//                        printf("data from pollfds[%d]\n", i);
                        printw("\ndata from pollfds[%d]\n", i);
                        refresh();
                        if (handle_request(pollfds[i].fd, client_addrs[i], active_users) != 0) {
                            continue;
                        }

                    default:
                        // default, fall-through from POLLIN case
                        //close(pollfds[i].fd);
                        //log_event(LOG_FILENAME, client_addrs[i], "DISCONNECT", i);
                        //pollfds[i].fd = -1;
                        //pollfds[i].revents = 0;
                        //memset(client_addrs + i, 0, MAX_IP_ADD_STR_LENGTH);
                        break;
                }
#pragma GCC diagnostic pop
            }
        }
    }
    for (int i = 1; i < MAX_CLIENTS; i++) {
        close(pollfds[i].fd);
    }
    close(opts->server_sock);
    return 0;
}

int setup_db_paths(void)
{
    memset(DB_USER_ACCOUNT_PATH, '\0', PATH_MAX);
    memset(DB_LOGIN_INFO_PATH, '\0', PATH_MAX);
    memset(DB_DISPLAY_NAMES_PATH, '\0', PATH_MAX);
    memset(DB_CHANNEL_INFO_PATH, '\0', PATH_MAX);
    memset(DB_MESSAGE_INFO_PATH, '\0', PATH_MAX);

    char temp_login_info_path[PATH_MAX] = { '\0', };
    char temp_display_names_path[PATH_MAX] = { '\0', };
    char temp_user_account_info_path[PATH_MAX] = { '\0', };
    char temp_channel_info_path[PATH_MAX] = { '\0', };
    char temp_message_info_path[PATH_MAX] = { '\0', };

    const char* home_dir = getenv("HOME");
    if (home_dir == NULL) {
        return EXIT_FAILURE;
    }

    size_t home_dir_len = strlen(home_dir);
    if (home_dir_len + strlen((const char*) DB_DISPLAY_NAMES_PATH) + 1 >= PATH_MAX
        || home_dir_len + strlen((const char*) DB_LOGIN_INFO_PATH) + 1 >= PATH_MAX
        || home_dir_len + strlen((const char*) DB_USER_ACCOUNT_PATH) + 1 >= PATH_MAX
        || home_dir_len + strlen((const char*) DB_CHANNEL_INFO_PATH) + 1 >= PATH_MAX
        || home_dir_len + strlen((const char*) DB_MESSAGE_INFO_PATH) + 1 >= PATH_MAX) {
        return EXIT_FAILURE;
    }

    strcpy(temp_login_info_path, home_dir);
    strcpy(temp_display_names_path, home_dir);
    strcpy(temp_user_account_info_path, home_dir);
    strcpy(temp_channel_info_path, home_dir);
    strcpy(temp_message_info_path, home_dir);

    strcat(temp_login_info_path, "/");
    strcat(temp_display_names_path, "/");
    strcat(temp_user_account_info_path, "/");
    strcat(temp_channel_info_path, "/");
    strcat(temp_message_info_path, "/");

    strcat(temp_login_info_path, DB_LOGIN_INFO);
    strcat(temp_display_names_path, DB_DISPLAY_NAMES);
    strcat(temp_user_account_info_path, DB_USER_ACCOUNT);
    strcat(temp_channel_info_path, DB_CHANNEL_INFO);
    strcat(temp_message_info_path, DB_MESSAGE_INFO);

    strncpy(DB_LOGIN_INFO_PATH, temp_login_info_path, PATH_MAX);
    strncpy(DB_DISPLAY_NAMES_PATH, temp_display_names_path, PATH_MAX);
    strncpy(DB_USER_ACCOUNT_PATH, temp_user_account_info_path, PATH_MAX);
    strncpy(DB_CHANNEL_INFO_PATH, temp_channel_info_path, PATH_MAX);
    strncpy(DB_MESSAGE_INFO_PATH, temp_message_info_path, PATH_MAX);

    return EXIT_SUCCESS;
}
