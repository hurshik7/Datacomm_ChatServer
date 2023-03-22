#include "db_viewer.h"
#include "util.h"
#include <ncurses.h>

void printAllUserAccounts(DBM *db)
{
    datum key, data;
    key = dbm_firstkey(db);
    while (key.dptr != NULL) {
        data = dbm_fetch(db, key);
        if (data.dptr != NULL) {
            struct UserAccountInfo* user = (struct UserAccountInfo *)data.dptr;
            char* clnt_addr = malloc(20);
            strncpy(clnt_addr, (const char*)&user->sock_addr, MAX_IP_ADD_STR_LENGTH);
            printw("%s, %s, %s, %d, %d\n", user->user_id, user->display_name,
                   clnt_addr, user->online_status, user->privilege_level);
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

void run_db_viewer(void)
{
    char *user_acc = malloc(sizeof(char) * 256);
    char *user_dsply_nm = malloc(sizeof(char) * 256);
    char *user_log_info = malloc(sizeof(char) * 256);
    struct utsname my_data = identify_os();
    char* my_os = my_data.sysname;
    if (strcmp(my_os, "Darwin") == 0) {
        strncpy(user_acc, DB_VIEWER_USER_ACCOUNT_LOC, strlen(DB_VIEWER_USER_ACCOUNT_LOC));
        strncpy(user_dsply_nm, DB_VIEWER_DISPLAY_NAMES_LOC, sizeof(DB_VIEWER_DISPLAY_NAMES_LOC));
        strncpy(user_log_info, DB_VIEWER_LOGIN_INFO_LOC, sizeof(DB_VIEWER_LOGIN_INFO_LOC));
    } else if (strcmp(my_os, "Linux") == 0) {
        strncpy(user_acc, DB_USER_ACCOUNT_LINUX, sizeof(DB_USER_ACCOUNT_LINUX));
        strncpy(user_dsply_nm, DB_DISPLAY_NAMES_LINUX, sizeof(DB_DISPLAY_NAMES_LINUX));
        strncpy(user_log_info, DB_LOGIN_INFO_LINUX, sizeof(DB_LOGIN_INFO_LINUX));
    }
    printw("UserAccounts DB\n");
    DBM* user_accounts = dbm_open(user_acc, O_RDWR | O_SYNC | O_APPEND, 0644);
    printAllUserAccounts(user_accounts);
    printw("\n");

    printw("DisplayNames DB\n");
    DBM* display_names = dbm_open(user_dsply_nm, O_RDWR | O_SYNC | O_APPEND, 0644);
    printAllDisplayNames(display_names);
    printw("\n");

    printw("UserLoginInfo DB\n");
    DBM* login_infos = dbm_open(user_log_info, O_RDWR | O_SYNC | O_APPEND, 0644);
    printAllLoginInfos(login_infos);
    printw("\n");
    refresh();

    // deallocate memory
    free(user_acc);
    free(user_dsply_nm);
    free(user_log_info);
    dbm_close(login_infos);
    dbm_close(display_names);
    dbm_close(user_accounts);
}
