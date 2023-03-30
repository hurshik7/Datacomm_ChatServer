#include "my_ndbm.h"
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <ncurses.h>


extern char DB_LOGIN_INFO_PATH[PATH_MAX];
extern char DB_DISPLAY_NAMES_PATH[PATH_MAX];
extern char DB_USER_ACCOUNT_PATH[PATH_MAX];


DBM* open_db_or_null(const char* db_name, int flag)
{
    DBM* db = dbm_open(db_name, flag, 0644);
    if (!db) {
//        perror("[DB]Error: Failed to open database");
        return NULL;
    }
    return db;
}

user_login_t* get_login_info_malloc_or_null(char* login_token)
{
    DBM* login_info_db = open_db_or_null(DB_LOGIN_INFO_PATH, O_RDONLY | O_SYNC);
    if (login_info_db == NULL) {
//        perror("[DB]Error: Failed to open LOGIN_INFO DB");
        return NULL;
    }

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = login_token;
    key.dsize = strlen(login_token);

    value = dbm_fetch(login_info_db, key);
    if (value.dptr == NULL) {
        dbm_close(login_info_db);
        perror("[DB]Error: User not found.");
        return NULL;
    }

    user_login_t* login_info = (user_login_t*) malloc(sizeof(user_login_t));
    if (login_info == NULL) {
        dbm_close(login_info_db);
        perror("[DB]Error: malloc()");
        return NULL;
    }

    memcpy(login_info, value.dptr, sizeof(user_login_t));
    dbm_close(login_info_db);
    return login_info;
}

user_account_t* get_user_account_malloc_or_null(char* user_token)
{
    DBM* user_acc_db = open_db_or_null(DB_USER_ACCOUNT_PATH, O_RDONLY | O_SYNC);
    if (user_acc_db == NULL) {
        return NULL;
    }
    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    // delete this
    printw("get_user_account() user_token: %s\n", user_token);
    refresh();
    // mising 1 character here

    key.dptr = user_token;
    key.dsize = strlen(user_token) + 1;

    value = dbm_fetch(user_acc_db, key);
    if (value.dptr == NULL) {
        dbm_close(user_acc_db);
        perror("[DB]Error: User not found.");
        return NULL;
    }

    user_account_t* user_acc = (user_account_t*) malloc(sizeof(user_account_t));
    if (user_acc == NULL) {
        dbm_close(user_acc_db);
        perror("[DB]Error: malloc()");
        return NULL;
    }

    memcpy(user_acc, value.dptr, sizeof(user_login_t));
    dbm_close(user_acc_db);
    return user_acc;
}

bool check_duplicate_display_name(char* display_name)
{
    DBM* display_names = open_db_or_null(DB_DISPLAY_NAMES_PATH, O_RDONLY | O_SYNC);
    if (display_names == NULL) {
//        perror("[DB]Error: Failed to open DB_DISPLAY_NAMES DB");
        return false;
    }

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = display_name;
    key.dsize = strlen(display_name);

    value = dbm_fetch(display_names, key);
    if (value.dptr == NULL) {
        dbm_close(display_names);
        return false;
    }

    dbm_close(display_names);
    return true;
}

int insert_user_account(user_account_t* user_account)
{
    DBM* user_accounts = open_db_or_null(DB_USER_ACCOUNT_PATH, O_CREAT | O_RDWR | O_SYNC | O_APPEND);
    if (user_accounts == NULL) {
        perror("[DB]Error: Failed to open DB_USER_ACCOUNT DB");
        return -1;
    }

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = user_account->user_id;
    key.dsize = strlen(user_account->user_id);
    value.dptr = user_account;
    value.dsize = sizeof(user_account_t);

    if (dbm_store(user_accounts, key, value, DBM_REPLACE) != 0) {
        perror("[DB]Error: Failed to insert user account\n");
        dbm_close(user_accounts);
        return -1;
    }

    dbm_close(user_accounts);
    return 0;
}

int insert_display_name(char* display_name, char* uuid)
{
    DBM* display_names = open_db_or_null(DB_DISPLAY_NAMES_PATH, O_CREAT | O_RDWR | O_SYNC | O_APPEND);
    if (display_names == NULL) {
        perror("[DB]Error: Failed to open DB_DISPLAY_NAMES DB");
        return -1;
    }

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = display_name;
    key.dsize = strlen(display_name);
    value.dptr = uuid;
    value.dsize = UUID_LEN;

    if (dbm_store(display_names, key, value, DBM_REPLACE) != 0) {
        perror("[DB]Error: Failed to insert display_name\n");
        dbm_close(display_names);
        return -1;
    }

    dbm_close(display_names);
    return 0;
}

int insert_user_login(user_login_t* user_login)
{
    DBM* user_logins = open_db_or_null(DB_LOGIN_INFO_PATH, O_CREAT | O_RDWR | O_SYNC | O_APPEND);
    if (user_logins == NULL) {
        perror("[DB]Error: Failed to open DB_LOGIN_INFO DB");
        return -1;
    }

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = user_login->login_token;
    key.dsize = strlen(user_login->login_token);
    value.dptr = user_login;
    value.dsize = sizeof(user_login_t);

    if (dbm_store(user_logins, key, value, DBM_REPLACE) != 0) {
        perror("[DB]Error: Failed to insert user login information\n");
        dbm_close(user_logins);
        return -1;
    }

    dbm_close(user_logins);
    return 0;
}
