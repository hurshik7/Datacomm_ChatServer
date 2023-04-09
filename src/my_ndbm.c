#include "my_ndbm.h"
#include <fcntl.h>
#include <limits.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>


// replace print(), perror() with printw() + refresh()
//#define printf(...) do { \
//    printw(__VA_ARGS__); \
//    refresh(); \
//} while (0)
#define perror(...) do { \
    printw(__VA_ARGS__); \
    refresh(); \
} while (0)


extern char DB_LOGIN_INFO_PATH[PATH_MAX];
extern char DB_DISPLAY_NAMES_PATH[PATH_MAX];
extern char DB_USER_ACCOUNT_PATH[PATH_MAX];
extern char DB_CHANNEL_INFO_PATH[PATH_MAX];
extern char DB_MESSAGE_INFO_PATH[PATH_MAX];

/* open DB */
DBM* open_db_or_null(const char* db_name, int flag)
{
    DBM* db = dbm_open(db_name, flag, 0644);
    if (!db) {
        //perror("[DB]Error: Failed to open database");
        return NULL;
    }
    return db;
}

/* Get functions */
user_login_t* get_login_info_malloc_or_null(char* login_token)
{
    DBM* login_info_db = open_db_or_null(DB_LOGIN_INFO_PATH, O_RDONLY | O_SYNC);
    if (login_info_db == NULL) {
        //perror("[DB]Error: Failed to open LOGIN_INFO DB");
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
        perror("[DB]Error: User not found - login.");
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

user_account_t* get_user_account_malloc_or_null(char* user_uuid)
{
    DBM* user_acc_db = open_db_or_null(DB_USER_ACCOUNT_PATH, O_RDONLY | O_SYNC);
    if (user_acc_db == NULL) {
        return NULL;
    }
    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = user_uuid;
    key.dsize = UUID_LEN;

    value = dbm_fetch(user_acc_db, key);
    if (value.dptr == NULL) {
        dbm_close(user_acc_db);
        perror("[DB]Error: User not found - user_account.");
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

char* get_uuid_with_display_name_or_null(char* display_name)
{
    DBM* display_names = open_db_or_null(DB_DISPLAY_NAMES_PATH, O_RDONLY | O_SYNC);
    if (display_names == NULL) {
        //perror("[DB]Error: Failed to open DB_DISPLAY_NAMES DB");
        return NULL;
    }

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = display_name;
    key.dsize = strlen(display_name);

    value = dbm_fetch(display_names, key);
    if (value.dptr == NULL) {
        dbm_close(display_names);
        return NULL;
    }

    dbm_close(display_names);
    return (char*) value.dptr;
}

message_info_t* get_message_malloc_or_null(char* user_token)
{
    DBM* message_db = open_db_or_null(DB_MESSAGE_INFO_PATH, O_RDONLY | O_SYNC);
    if (message_db == NULL) {
        return NULL;
    }
    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = user_token;
    key.dsize = strlen(user_token);

    value = dbm_fetch(message_db, key);
    if (value.dptr == NULL) {
        dbm_close(message_db);
        perror("[DB]Error: Message not found.");
        return NULL;
    }

    message_info_t * message = (message_info_t *) malloc(sizeof(message_info_t));
    if (message == NULL) {
        dbm_close(message_db);
        perror("[DB]Error: malloc()");
        return NULL;
    }

    memcpy(message, value.dptr, sizeof(message_info_t));
    dbm_close(message_db);
    return message;
}

channel_info_t* get_channel_info_malloc_or_null(char* channel_name)
{
    DBM* channel_infos = open_db_or_null(DB_CHANNEL_INFO_PATH, O_RDONLY | O_SYNC);
    if (channel_infos == NULL) {
        //perror("[DB]Error: Failed to open DB_CHANNEL_INFO DB");
        return NULL;
    }

    datum key, value;
    // Iterate through all keys and values
    for (key = dbm_firstkey(channel_infos); key.dptr != NULL; key = dbm_nextkey(channel_infos)) {
        value = dbm_fetch(channel_infos, key);
        if (value.dptr == NULL) {
            perror("dbm_fetch");
            dbm_close(channel_infos);
            return NULL;
        }
        channel_info_t* k_channel = (channel_info_t*) value.dptr;
        if (strcmp(k_channel->channel_name, channel_name) == 0) {
            channel_info_t* ret_channel_info = (channel_info_t*) malloc(sizeof(channel_info_t));
            memcpy(ret_channel_info, k_channel, sizeof(channel_info_t));
            return ret_channel_info;
        }
    }

    dbm_close(channel_infos);
    return NULL;
}

/* Check duplicates functions */
bool check_duplicate_display_name(char* display_name)
{
    DBM* display_names = open_db_or_null(DB_DISPLAY_NAMES_PATH, O_RDONLY | O_SYNC);
    if (display_names == NULL) {
        //perror("[DB]Error: Failed to open DB_DISPLAY_NAMES DB");
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

bool check_duplicate_channel_name(char* channel_name)
{
    DBM* channel_infos = open_db_or_null(DB_CHANNEL_INFO_PATH, O_RDONLY | O_SYNC);
    if (channel_infos == NULL) {
        //perror("[DB]Error: Failed to open DB_CHANNEL_INFO DB");
        return false;
    }

    datum key, value;
    // Iterate through all keys and values
    for (key = dbm_firstkey(channel_infos); key.dptr != NULL; key = dbm_nextkey(channel_infos)) {
        value = dbm_fetch(channel_infos, key);
        if (value.dptr == NULL) {
            perror("dbm_fetch");
            dbm_close(channel_infos);
            return false;
        }
        channel_info_t* k_channel = (channel_info_t*) value.dptr;
        if (strcmp(k_channel->channel_name, channel_name) == 0) {
            return true;
        }
    }

    dbm_close(channel_infos);
    return false;
}

/* Insert functions */
int insert_user_account(user_account_t* user_account)
{
    DBM* user_accounts = open_db_or_null(DB_USER_ACCOUNT_PATH, O_CREAT | O_RDWR | O_SYNC | O_APPEND);
    if (user_accounts == NULL) {
        //perror("[DB]Error: Failed to open DB_USER_ACCOUNT DB");
        return -1;
    }

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = user_account->user_id;
    key.dsize = UUID_LEN;
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
        //perror("[DB]Error: Failed to open DB_DISPLAY_NAMES DB");
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
        //perror("[DB]Error: Failed to insert display_name\n");
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
        //perror("[DB]Error: Failed to open DB_LOGIN_INFO DB");
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

int insert_channel_info(channel_info_t* channel_info)
{
    DBM* channel_infos = open_db_or_null(DB_CHANNEL_INFO_PATH, O_CREAT | O_RDWR | O_SYNC | O_APPEND);
    if (channel_infos == NULL) {
        //perror("[DB]Error: Failed to open DB_CHANNEL_INFO DB");
        return -1;
    }

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = channel_info->channel_id;
    key.dsize = UUID_LEN;
    value.dptr = channel_info;
    value.dsize = sizeof(channel_info_t);

    if (dbm_store(channel_infos, key, value, DBM_REPLACE) != 0) {
        perror("[DB]Error: Failed to insert channel information\n");
        dbm_close(channel_infos);
        return -1;
    }

    dbm_close(channel_infos);
    return 0;
}

int insert_message(message_info_t * message)
{
    DBM* user_messages = open_db_or_null(DB_MESSAGE_INFO_PATH, O_CREAT | O_RDWR | O_SYNC | O_APPEND);
    if (user_messages == NULL) {
        //perror("[DB]Error: Failed to open DB_MESSAGES DB");
        return -1;
    }

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = message->message_id;
    key.dsize = UUID_LEN;
    value.dptr = message;
    value.dsize = sizeof(message_info_t);

    if (dbm_store(user_messages, key, value, DBM_REPLACE) != 0) {
        perror("[DB]Error: Failed to insert user login information\n");
        dbm_close(user_messages);
        return -1;
    }

    dbm_close(user_messages);
    return 0;
}

/* Remove functions */
int remove_user_account(char* user_id)
{
    DBM* user_accounts = open_db_or_null(DB_USER_ACCOUNT_PATH, O_CREAT | O_RDWR | O_SYNC | O_APPEND);
    if (user_accounts == NULL) {
        //perror("[DB]Error: Failed to open DB_USER_ACCOUNT DB");
        return -1;
    }

    datum key;
    memset(&key, 0, sizeof(datum));

    key.dptr = user_id;
    key.dsize = UUID_LEN;

    if (dbm_delete(user_accounts, key) != 0) {
        perror("[DB]Error: Failed to remove user account\n");
        dbm_close(user_accounts);
        return -1;
    }

    dbm_close(user_accounts);
    return 0;
}

int remove_display_name(char* display_name)
{
    DBM* display_names = open_db_or_null(DB_DISPLAY_NAMES_PATH, O_CREAT | O_RDWR | O_SYNC | O_APPEND);
    if (display_names == NULL) {
        //perror("[DB]Error: Failed to open DB_DISPLAY_NAMES DB");
        return -1;
    }

    datum key;
    memset(&key, 0, sizeof(datum));

    key.dptr = display_name;
    key.dsize = strlen(display_name);

    if (dbm_delete(display_names, key) != 0) {
        perror("[DB]Error: Failed to remove display_name\n");
        dbm_close(display_names);
        return -1;
    }

    dbm_close(display_names);
    return 0;
}

int remove_user_login(char* login_token)
{
    DBM* user_logins = open_db_or_null(DB_LOGIN_INFO_PATH, O_CREAT | O_RDWR | O_SYNC | O_APPEND);
    if (user_logins == NULL) {
        //perror("[DB]Error: Failed to open DB_LOGIN_INFO DB");
        return -1;
    }

    datum key;
    memset(&key, 0, sizeof(datum));

    key.dptr = login_token;
    key.dsize = strlen(login_token);

    if (dbm_delete(user_logins, key) != 0) {
        perror("[DB]Error: Failed to remove user login information\n");
        dbm_close(user_logins);
        return -1;
    }

    dbm_close(user_logins);
    return 0;
}

/* Update functions */
int update_channel_name_and_publicity(char* channel_name, char* new_channel_name, bool new_publicity)
{
    DBM* channel_info_db = open_db_or_null(DB_CHANNEL_INFO_PATH, O_RDWR | O_SYNC);
    if (channel_info_db == NULL) {
        return -1;
    }

    channel_info_t* fetched_channel = get_channel_info_malloc_or_null(channel_name);
    if (fetched_channel == NULL) {
        dbm_close(channel_info_db);
        return -1;
    }

    strncpy(fetched_channel->channel_name, new_channel_name, TOKEN_NAME_LENGTH);
    fetched_channel->publicity = new_publicity;

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = fetched_channel->channel_id;
    key.dsize = UUID_LEN;
    value.dptr = fetched_channel;
    value.dsize = sizeof(channel_info_t);
    if (dbm_store(channel_info_db, key, value, DBM_REPLACE) != 0) {
        dbm_close(channel_info_db);
        return -1;
    }

    dbm_close(channel_info_db);
    free(fetched_channel);
    return 0;
}

int add_users_on_channel(char* channel_name, char users_to_add[DEFAULT_LIST_SIZE][TOKEN_NAME_LENGTH])
{
    DBM* channel_info_db = open_db_or_null(DB_CHANNEL_INFO_PATH, O_RDWR | O_SYNC);
    if (channel_info_db == NULL) {
        return -1;
    }

    channel_info_t* fetched_channel = get_channel_info_malloc_or_null(channel_name);
    if (fetched_channel == NULL) {
        return -1;
    }

    int i = 0;
    int j = 0;
    while (j < DEFAULT_LIST_SIZE && fetched_channel->user_list[j][0] != '\0') {
        j++;
    }
    // does not check if a user already exists before adding them
    while (j < DEFAULT_LIST_SIZE && i < DEFAULT_LIST_SIZE && users_to_add[i][0] != '\0') {
        strncpy(fetched_channel->user_list[j++], users_to_add[i], TOKEN_NAME_LENGTH);
        i++;
    }

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = fetched_channel->channel_id;
    key.dsize = UUID_LEN;
    value.dptr = fetched_channel;
    value.dsize = sizeof(channel_info_t);

    if (dbm_store(channel_info_db, key, value, DBM_REPLACE) != 0) {
        dbm_close(channel_info_db);
        return -1;
    }

    dbm_close(channel_info_db);
    free(fetched_channel);
    return 0;
}
