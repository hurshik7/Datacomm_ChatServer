#ifndef CHAT_SERVER_NCURSES_UI_H
#define CHAT_SERVER_NCURSES_UI_H


#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include "server.h"


#define TITLE "Chat Server"

#define MENU_HEIGHT (11)
#define MENU_WIDTH (80)
#define DB_PATH_BOX_HEIGHT (9)
#define DB_PATH_BOX_WIDTH (60)


/**
 * Prints the menu box.
 * @param menu_win a WINDOW pointer
 * @param highlight an integer
 */
void print_menu(WINDOW* menu_win, int highlight);

/**
 * Creates a window for the menu box.
 * @return a WINDOW pointer
 */
WINDOW* create_menu_window(void);

/**
 * Create a window for the DB path box.
 * @return a Window pointer
 */
WINDOW* create_db_path_window(void);

/**
 * Get user's choice to select an option.
 * @param menu_win a WINDOW pointer
 * @return an integer represents the user's choice
 */
int navigate_menu(WINDOW* menu_win);

/**
 * Prints the title of this program.
 * @param startx an integer represents the start location of x
 */
void print_title(int startx);

/**
 * Prints the paths of the db files.
 * @param db_win a WINDOW pointer
 * @param startx an integer represents the start location of x
 */
void print_db_paths(WINDOW* db_win, int startx);

/**
 * Initiate ncurses ui.
 */
void init_ncurses(void);

/**
 * Runs the db_viewer thread.
 */
void run_db_viewer_wrapper(void);

/**
 * Runs the active_user_viewer thread.
 * @param cache a connected_user array
 */
void run_active_user_viewer(connected_user* cache);


#endif //CHAT_SERVER_NCURSES_UI_H

