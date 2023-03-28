#ifndef CHAT_SERVER_NCURSES_UI_H
#define CHAT_SERVER_NCURSES_UI_H


#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include "server.h"


#define TITLE "Chat Server"


void print_menu(WINDOW* menu_win, int highlight);
WINDOW* create_menu_window(void);
int navigate_menu(WINDOW* menu_win);
void print_title(int startx);
void init_ncurses(void);
void run_db_viewer_wrapper(void);
void run_active_user_viewer(connected_user* cache);


#endif //CHAT_SERVER_NCURSES_UI_H
