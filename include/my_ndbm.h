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


typedef struct UserLoginInfo {
    char login_token[TOKEN_NAME_LENGTH];
    char password[PSWD_MAX_LENGTH];
    char uuid[UUID_LEN];
} user_login_t;

typedef struct UserAccountInfo {
    char user_id[UUID_LEN];
    char display_name[TOKEN_NAME_LENGTH];
    struct sockaddr_in sock_addr;
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
    char* message_content;
    uint8_t time_stamp[TIMESTAMP_SIZE];
} message_info_t;

DBM* open_db_or_null(const char* db_name, int flag);
user_login_t* get_login_info_malloc_or_null(char *login_token);
user_account_t* get_user_account_malloc_or_null(char* user_token);
message_info_t* get_message_malloc_or_null(char* user_token);
bool check_duplicate_display_name(char* display_name);
int insert_user_account(user_account_t* user_account);
int insert_display_name(char* display_name, char* uuid);
int insert_user_login(user_login_t* user_login);
int insert_message(message_info_t * message);


#endif /* MY_NDBM_H */

