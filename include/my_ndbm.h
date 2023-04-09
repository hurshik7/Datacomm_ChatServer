#ifndef MY_NDBM_H
#define MY_NDBM_H


#include <ndbm.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <uuid/uuid.h>


#define TOKEN_NAME_LENGTH (21)
#define PSWD_MAX_LENGTH (31)
#define DSPLY_NAME_LENGTH (21)
#define UUID_LEN (37)
#define CLNT_IP_ADDR_LENGTH (16)
#define DEFAULT_LIST_SIZE (64)
#define TIMESTAMP_SIZE (8)
#define MAX_MESSAGE_SIZE (974)


#define DB_LOGIN_INFO "user_login_info"
#define DB_DISPLAY_NAMES "display_names"
#define DB_USER_ACCOUNT "user_account_info"
#define DB_CHANNEL_INFO "channel_info"
#define DB_MESSAGE_INFO "messages_info"


typedef struct UserLoginInfo {
    char login_token[TOKEN_NAME_LENGTH];
    char password[PSWD_MAX_LENGTH];
    char uuid[UUID_LEN];
} user_login_t;

typedef struct UserAccountInfo {
    char user_id[UUID_LEN];
    char display_name[TOKEN_NAME_LENGTH];
    char ip_addr[CLNT_IP_ADDR_LENGTH];
    bool online_status;
    int privilege_level;
} user_account_t;

typedef struct ChannelInfo {
    char channel_id[UUID_LEN];
    char creator[TOKEN_NAME_LENGTH];
    char user_list[DEFAULT_LIST_SIZE][TOKEN_NAME_LENGTH];
    char admin_list[DEFAULT_LIST_SIZE][TOKEN_NAME_LENGTH];
    char banned_list[DEFAULT_LIST_SIZE][TOKEN_NAME_LENGTH];
    char channel_name[TOKEN_NAME_LENGTH];
    bool publicity;
} channel_info_t;

typedef struct MessageInfo {
    char message_id[UUID_LEN];
    char user_id[UUID_LEN];
    char channel_id[UUID_LEN];
    char message_content[MAX_MESSAGE_SIZE];
    uint8_t time_stamp[TIMESTAMP_SIZE];
} message_info_t;

/* open DB */
DBM* open_db_or_null(const char* db_name, int flag);

/* Get functions */
user_login_t* get_login_info_malloc_or_null(char *login_token);
user_account_t* get_user_account_malloc_or_null(char* user_uuid);
char* get_uuid_with_display_name_or_null(char* display_name);
message_info_t* get_message_malloc_or_null(char* user_token);
channel_info_t* get_channel_info_malloc_or_null(char* channel_name);

/* Check functions */
bool check_duplicate_display_name(char* display_name);
bool check_duplicate_channel_name(char* channel_name);
bool check_if_user_in_channel(const char* display_name, const channel_info_t* channel);

/* Insert functions */
int insert_user_account(user_account_t* user_account);
int insert_display_name(char* display_name, char* uuid);
int insert_user_login(user_login_t* user_login);
int insert_channel_info(channel_info_t* channel_info);
int insert_message(message_info_t * message);

/* Remove functions */
int remove_user_account(char* user_id);
int remove_display_name(char* display_name);
int remove_user_login(char* login_token);

/* Update functions */
int update_channel_name_and_publicity(char* channel_name, char* new_channel_name, bool new_publicity);
int add_users_on_channel(char* channel_name, char users_to_add[DEFAULT_LIST_SIZE][TOKEN_NAME_LENGTH]);


#endif /* MY_NDBM_H */

