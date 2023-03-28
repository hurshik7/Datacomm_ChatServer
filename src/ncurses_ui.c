#include "db_viewer.h"
#include "ncurses_ui.h"
#include <pthread.h>


extern bool server_running;
extern pthread_t server_thread;


void print_menu(WINDOW *menu_win, int highlight)
{
    int x, y, i;
    const char* choices[] = {
            "1. Run server",
            "2. View database",
            "3. Quit",
    };

    x = 2;
    y = 2;

    box(menu_win, 0, 0);

    for (i = 0; i < 3; i++) {
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

WINDOW* create_menu_window(void)
{
    int menu_height = 9;
    int menu_width = 80;
    int menu_startx = (COLS - menu_width) / 2;
    int menu_starty = (LINES - menu_height) / 2;
    WINDOW *menu_win = newwin(menu_height, menu_width, menu_starty, menu_startx);
    return menu_win;
}

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
                    choice = 3;
                }
                break;
            case KEY_DOWN:
                choice++;
                if (choice > 3) {
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

void print_title(int startx)
{
    attron(A_BOLD);
    mvprintw(1, startx, TITLE);
    attroff(A_BOLD);
    refresh();
}

void init_ncurses(void)
{
    // Initialize ncurses
    initscr();
    clear();
    noecho();
    cbreak();
}

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



//Version 1 of our gui
//to run ncurses
// gcc -o hello main.c -lncurses
//int main(void)
//{
//    // START NCURSES
//    initscr();
//    cbreak();
//
//    //Max screen size
//    int y_Max, x_Max;
//    getmaxyx(stdscr, y_Max, x_Max);
//
//    //create window for input
//    WINDOW * menuwin = newwin(7, x_Max - 12, y_Max - 8, 5);
//    box(menuwin, 0, 0);
//    refresh();
//    wrefresh(menuwin);
//
//    //Enable arrow keys
//    keypad(menuwin, true);
//
//    char choices[5][256] = {"Start Server", "End Server", "Show Database", "Admin", "Exit"};
//
//    int choice;
//    int highlight = 0;
//
//    while(1)
//    {
//        for (int i = 0; i < 4; i++)
//        {
//            if (i == highlight)
//                wattron(menuwin, A_STANDOUT);
//                mvwprintw(menuwin, i + 1, 1, choices[i]);
//                wattroff(menuwin, A_STANDOUT);
//        }
//            choice = wgetch(menuwin);
//            switch(choice)
//            {
//                case KEY_UP:
//                    highlight--;
//                    if(highlight == -1)
//                    {
//                        highlight = 0;
//                    }
//                    break;
//                case KEY_DOWN:
//                    highlight++;
//                    if(highlight == 4)
//                    {
//                        highlight = 3;
//                    }
//                    break;
//                default:
//                    break;
//            }
//            if(choice == 10)
//            {
//                break;
//            }
//        }
//
//    if (strcmp(choices[highlight], choices[0]) == 0)
//    {
//        printw("Server is running...");
//        run_server(argc,argv);
//    }
//    printw("Your choice was: %s", choices[highlight]);
//    //Ensuring program waits before exiting
//    getch();
//    endwin();
//
//
//    return 0;
//}

