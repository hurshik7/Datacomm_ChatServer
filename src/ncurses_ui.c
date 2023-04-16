#include "db_viewer.h"
#include "ncurses_ui.h"
#include "server.h"
#include <limits.h>
#include <pthread.h>


extern bool server_running;

extern char DB_LOGIN_INFO_PATH[PATH_MAX];
extern char DB_DISPLAY_NAMES_PATH[PATH_MAX];
extern char DB_USER_ACCOUNT_PATH[PATH_MAX];
extern char DB_CHANNEL_INFO_PATH[PATH_MAX];
extern char DB_MESSAGE_INFO_PATH[PATH_MAX];

/**
 * prints the menu box.
 * @param menu_win a WINDOW pointer.
 * @param highlight an integer.
 */
void print_menu(WINDOW *menu_win, int highlight)
{
    int x, y, i;
    const char* choices[] = {
            "1. Run server",
            "2. View database",
            "3. View Active Users",
            "4. Quit",
    };

    x = 3;
    y = 3;

    box(menu_win, 0, 0);

    for (i = 0; i < 4; i++) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
        y++;
    }

    y++;
    mvwprintw(menu_win, y, x, "Created by Shik Hur, Benny Chao, David Lee (BCIT CST COMP4985 Project)");
    wrefresh(menu_win);
}

/**
 * create a window for the menu box.
 * @return a WINDOW pointer.
 */
WINDOW* create_menu_window(void)
{
    int menu_startx = (COLS - MENU_WIDTH) / 2;
    int menu_starty = (LINES - MENU_HEIGHT) / 2;
    WINDOW *menu_win = newwin(MENU_HEIGHT, MENU_WIDTH, menu_starty, menu_startx);
    return menu_win;
}

/**
 * creates a window for the database path.
 * @return a WINDOW pointer.
 */
WINDOW* create_db_path_window(void)
{
    int menu_startx = (COLS - DB_PATH_BOX_WIDTH) / 2;
    int menu_starty = 5;
    WINDOW *db_win = newwin(DB_PATH_BOX_HEIGHT, DB_PATH_BOX_WIDTH, menu_starty, menu_startx);
    return db_win;
}

/**
 * gets the User's input to select an option in the menu.
 * @param menu_win a WINDOW pointer.
 * @return an integer representing the user's choice.
 */
int navigate_menu(WINDOW *menu_win)
{
    int choice = 1;
    int key;

    while (1) {
        print_menu(menu_win, choice);

        key = wgetch(menu_win);
        switch (key) {
            case KEY_UP:
                choice--;
                if (choice < 1) {
                    choice = 4;
                }
                break;
            case KEY_DOWN:
                choice++;
                if (choice > 4) {
                    choice = 1;
                }
                break;
            case 10:  // Enter key
                return choice;
            default:
                break;
        }
    }
}

/**
 * prints the title.
 * @param startx an integer representing the start location of x
 */
void print_title(int startx)
{
    attron(A_BOLD);
    mvprintw(1, startx, TITLE);
    attroff(A_BOLD);
    refresh();
}

/**
 * prints the path to database.
 * @param db_win  a WINDOW pointer
 * @param startx  an integer representing the start location of x
 */
void print_db_paths(WINDOW* db_win, int startx)
{
    int y = 1;
    box(db_win, 0, 0);
    mvwprintw(db_win, y++, startx, "DB paths");
    y++;
    mvwprintw(db_win, y++, startx, "%s: %s", "User Login info", DB_LOGIN_INFO_PATH);
    mvwprintw(db_win, y++, startx, "%s: %s", "Display names", DB_DISPLAY_NAMES_PATH);
    mvwprintw(db_win, y++, startx, "%s: %s", "User account", DB_USER_ACCOUNT_PATH);
    mvwprintw(db_win, y++, startx, "%s: %s", "Channel info", DB_CHANNEL_INFO_PATH);
    mvwprintw(db_win, y, startx, "%s: %s", "Message info", DB_MESSAGE_INFO_PATH);
    wrefresh(db_win);
}

/**
 * Initialize ncurses UI.
 */
void init_ncurses(void)
{
    // Initialize ncurses
    initscr();
    clear();
    noecho();
    cbreak();
}

/**
 * Runs the database viewer thread.
 */
void run_db_viewer_wrapper(void)
{
    // Clear the screen
    clear();
    refresh();

    printw("Database contents:\n");
    refresh();
    run_db_viewer();

    // Wait for a key press to close the database view
    getch();

    // Clear the screen again
    clear();
    refresh();
}

/**
 * Runs the active user thread.
 * @param users a cache containing the current active users
 */
void run_active_user_viewer(connected_user* users)
{
    // Clear screen
    clear();
    refresh();

    view_active_users(users);

    //Wait for a key press to close active user view
    getch();

    //Clear screen.
    clear();
    refresh();

}
