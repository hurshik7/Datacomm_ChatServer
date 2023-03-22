#include <ncurses.h>
#include <string.h>
#include "error.h"
//#include "~/src/main-chat-server.c"

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
