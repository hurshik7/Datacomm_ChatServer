#ifndef CHAT_SERVER_SERVER_H
#define CHAT_SERVER_SERVER_H


#include "my_ndbm.h"
#include <stdint.h>


#define DEFAULT_BUFFER (1024)
#define HEADER_SIZE (4)
#define CURRENT_VERSION (1)
#define ERROR_WRONG_VERSION (2)

#define TYPE_CREATE (1)
#define TYPE_READ (2)
#define TYPE_UPDATE (3)
#define TYPE_DESTROY (4)

#define OBJECT_USER (1)
#define OBJECT_CHANNEL (2)
#define OBJECT_MESSAGE (3)
#define OBJECT_AUTH (4)

#define ERROR_CREATE_USER_DUPLICATE_ALL (1)
#define ERROR_CREATE_USER_DUPLICATE_TOKEN (2)
#define ERROR_CREATE_USER_DUPLICATE_DISPLAY_NAME (3)

#define ERROR_LOGIN_INVALID_CREDENTIALS (1)
#define ERROR_LOGIN_DOES_NOT_EXIST (2)
#define TERMINATE_AND_RESTABLISH_CONNECTION (9)

#define ERROR_LOGOUT_INVALID_FIELDS (1)
#define ERROR_LOGOUT_USER_MISMATCH_ADDRESS (2)
#define ERROR_LOGOUT_ADMIN_USER_NOT_EXIST (3)
#define ERROR_LOGOUT_ADMIN_USER_NOT_ONLINE (4)

#define ERROR_INVALID_REQUEST (1)
#define ERROR_CHANNEL_DOES_NOT_EXIST (2)
#define ERROR_ADMIN_DOES_NOT_EXIST (3)
#define ERROR_USER_DOES_NOT_EXIST (4)

typedef struct version_type {
    uint8_t  version:4;
    uint8_t  type:4;
} version_type_t;

typedef struct chat_header {
    version_type_t version_type;
    uint8_t        object;
    uint16_t       body_size;
} chat_header_t;

typedef struct {
    int fd;
    char* dsply_name;
    char* ip_address;
    time_t access_time;
} connected_user;

int handle_request(int fd, const char* clnt_addr, connected_user* cache);
int read_header(int fd, chat_header_t *header_out);
int read_and_create_user(int fd, char token_out[TOKEN_NAME_LENGTH], uint16_t body_size);
int read_and_login_user(int fd, char token_out[TOKEN_NAME_LENGTH], const char* clnt_addr, connected_user* cache);
int read_and_logout_user(int fd, char token_out[TOKEN_NAME_LENGTH], const char* clnt_addr, connected_user* cache);
int read_and_create_message(int fd, char token_out[TOKEN_NAME_LENGTH]);
user_login_t* generate_user_login_malloc_or_null(const char* login_token, const char* password, const char* user_id);
user_account_t* generate_user_account_malloc_or_null(const char* uuid, const char* display_name);
user_account_t* login_user_account_malloc_or_null(user_account_t* user_acc, const char* clnt_addr);
user_account_t* logout_user_account_malloc_or_null(user_account_t* user_acc);
int send_create_user_response(int fd, chat_header_t header, int result, const char* token, const char* clnt_addr);
int send_login_user_response(int fd, chat_header_t header, int result, const char* token, const char* clnt_addr);
int send_logout_user_response(int fd, chat_header_t header, int result, const char* token, const char* clnt_addr);
int send_create_message_response(int fd, chat_header_t, int result, const char* token, const char* clnt_addr);
int get_num_connected_users(connected_user* cache);
void insert_user_in_cache(int fd, connected_user* cache, user_account_t* connecting_user, int num_active_users);
void remove_user_in_cache(connected_user* cache, user_account_t* connecting_user, int num_active_users);
int cmp_users(const void* a, const void* b);
int find_duplicate_user(connected_user* users, int n);
bool find_connected_user_with_same_cred(user_account_t* user_account, connected_user* conn_users, int num_users, int fd);
uint32_t create_response_header(const chat_header_t* header);
void view_active_users(connected_user* cache);

#endif //CHAT_SERVER_SERVER_H
