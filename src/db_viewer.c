#include "db_viewer.h"
#include <limits.h>
#include <ncurses.h>


extern char DB_LOGIN_INFO_PATH[PATH_MAX];
extern char DB_DISPLAY_NAMES_PATH[PATH_MAX];
extern char DB_USER_ACCOUNT_PATH[PATH_MAX];
extern char DB_CHANNEL_INFO_PATH[PATH_MAX];


void printAllUserAccounts(DBM *db)
{
    datum key, data;
    key = dbm_firstkey(db);
    while (key.dptr != NULL) {
        data = dbm_fetch(db, key);
        if (data.dptr != NULL) {
            struct UserAccountInfo* user = (struct UserAccountInfo *)data.dptr;
            printw("%s, %s, %s, %d, %d\n", user->user_id, user->display_name,
                   user->ip_addr, user->online_status, user->privilege_level);
        }
        key = dbm_nextkey(db);
    }
}

void printAllDisplayNames(DBM *db)
{
    datum key, data;
    key = dbm_firstkey(db);
    while (key.dptr != NULL) {
        data = dbm_fetch(db, key);
        if (data.dptr != NULL) {
            char* user_uuid = (char*) data.dptr;
            printw("%s, %s\n", (char*) key.dptr, user_uuid);
        }
        key = dbm_nextkey(db);
    }
}

void printAllLoginInfos(DBM *db)
{
    datum key, data;
    key = dbm_firstkey(db);
    while (key.dptr != NULL) {
        data = dbm_fetch(db, key);
        if (data.dptr != NULL) {
            user_login_t* user = (user_login_t*) data.dptr;
            printw("%s, %s, %s\n", user->login_token, user->password, user->uuid);
        }
        key = dbm_nextkey(db);
    }
}

void printAllChannelInfos(DBM *db)
{
    datum key, data;
    key = dbm_firstkey(db);
    while (key.dptr != NULL) {
        data = dbm_fetch(db, key);
        if (data.dptr != NULL) {
            channel_info_t* channel = (channel_info_t*) data.dptr;
            printw("%s, %s, %s\n", channel->channel_name, channel->creator, channel->channel_id);
            printw("user list: \n");
            int i = 0;
            while (channel->user_list[i][0] != '\0' && i < DEFAULT_LIST_SIZE) {
                printw("%s ", channel->user_list[i]);
                i++;
            }
            printw("\n");

            printw("admin list: \n");
            i = 0;
            while (channel->admin_list[i][0] != '\0' && i < DEFAULT_LIST_SIZE) {
                printw("%s ", channel->admin_list[i]);
                i++;
            }
            printw("\n");
        }
        key = dbm_nextkey(db);
    }
}

void run_db_viewer(void)
{
    printw("UserAccounts DB\n");
    DBM* user_accounts = dbm_open(DB_USER_ACCOUNT_PATH, O_RDWR | O_SYNC | O_APPEND, 0644);
    if (user_accounts != NULL) {
        printAllUserAccounts(user_accounts);
    }
    printw("\n");

    printw("DisplayNames DB\n");
    DBM* display_names = dbm_open(DB_DISPLAY_NAMES_PATH, O_RDWR | O_SYNC | O_APPEND, 0644);
    if (display_names != NULL) {
        printAllDisplayNames(display_names);
    }
    printw("\n");

    printw("UserLoginInfo DB\n");
    DBM* login_infos = dbm_open(DB_LOGIN_INFO_PATH, O_RDWR | O_SYNC | O_APPEND, 0644);
    if (login_infos != NULL) {
        printAllLoginInfos(login_infos);
    }
    printw("\n");

    printw("ChannelInfo DB\n");
    DBM* channel_infos = dbm_open(DB_CHANNEL_INFO_PATH, O_RDWR | O_SYNC | O_APPEND, 0644);
    if (channel_infos != NULL) {
        printAllChannelInfos(channel_infos);
    }
    printw("\n");
    refresh();

    // deallocate memory
    dbm_close(login_infos);
    dbm_close(display_names);
    dbm_close(user_accounts);
}
