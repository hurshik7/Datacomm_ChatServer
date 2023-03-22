#ifndef CHAT_SERVER_NCURSES_UI_H
#define CHAT_SERVER_NCURSES_UI_H


#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>


#define TITLE "Chat Server"


void print_menu(WINDOW* menu_win, int highlight);
WINDOW* create_menu_window(void);
int navigate_menu(WINDOW* menu_win);
void print_title(int startx);
void init_ncurses(void);
void view_database_wrapper(void);


#endif //CHAT_SERVER_NCURSES_UI_H
