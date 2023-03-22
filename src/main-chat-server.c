#include "error.h"
#include "option_handler.h"
#include "server.h"
#include "util.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>


#define BACKLOG (5)
#define MAX_CLIENTS (255)


static void init_pollfd(struct pollfd* pollfds, int server_sock);

connected_user active_users[MAX_CLIENTS]; // active user cache w/ upper limit of 255 concurrent clients

int main(int argc, char *argv[]) {
    struct sockaddr_in client_addr;
    int option;
    socklen_t socket_len;
    char client_addrs[MAX_CLIENTS][MAX_IP_ADD_STR_LENGTH];
    memset(client_addrs, 0, MAX_CLIENTS * MAX_IP_ADD_STR_LENGTH);

    // initiate options, and parse command line argument
    struct options opts;
    options_init(&opts);
    if (parse_arguments(argc, argv, &opts) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);                                              // NOLINT(concurrency-mt-unsafe)
    }

    // Create a socket for the serve
    opts.server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (opts.server_sock < 0) {
        fatal_errno(__FILE__, __func__ , __LINE__, errno, 2);
    }
    // set the option
    if (setsockopt(opts.server_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        close(opts.server_sock);
        fatal_errno(__FILE__, __func__ , __LINE__, errno, 2);
    }

    // Bind the socket to an address and port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(opts.port_in);
    if (bind(opts.server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        close(opts.server_sock);
        fatal_errno(__FILE__, __func__ , __LINE__, errno, 2);
    }

    // Listen for incoming connections
    if (listen(opts.server_sock, BACKLOG) < 0) {
        close(opts.server_sock);
        fatal_errno(__FILE__, __func__ , __LINE__, errno, 2);
    }

    // Initialize the pollfd structure for the poll-server socket
    struct pollfd pollfds[MAX_CLIENTS];
    init_pollfd(pollfds, opts.server_sock);
    fprintf(stdout, "Listening on port %d\n", opts.port_in);
    while (1) {
        // Call poll() to wait for events on the file descriptors
        int num_fds = MAX_CLIENTS;
        int result = poll(pollfds, num_fds, -1); // -1 means infinite waiting
        if (result > 0) {
            // Check if there's a new client connection
            if (pollfds[0].revents == POLLIN) {
                memset(&client_addr, 0, sizeof(struct sockaddr_in));
                int client_sock = accept(opts.server_sock, (struct sockaddr *) &client_addr, &socket_len);
                if (client_sock < 0) {
                    close(opts.server_sock);
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
//                        log_event(LOG_FILENAME, client_addrs[i], "CONNECT", i);
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

                    // data is ready
                    case POLLIN:
                        // read request
                        printf("data from pollfds[%d]\n", i);
                        if (handle_request(pollfds[i].fd, client_addrs[i], active_users) != 0) {
                            continue;
                        }

                     // default, fall-through from POLLIN case
                    default:
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

