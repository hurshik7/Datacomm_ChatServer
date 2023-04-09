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


void print_menu(WINDOW* menu_win, int highlight);
WINDOW* create_menu_window(void);
WINDOW* create_db_path_window(void);
int navigate_menu(WINDOW* menu_win);
void print_title(int startx);
void print_db_paths(WINDOW* db_win, int startx);
void init_ncurses(void);
void run_db_viewer_wrapper(void);
void run_active_user_viewer(connected_user* cache);


#endif //CHAT_SERVER_NCURSES_UI_H
