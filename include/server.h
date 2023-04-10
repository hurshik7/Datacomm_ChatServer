#ifndef CHAT_SERVER_SERVER_H
#define CHAT_SERVER_SERVER_H


#include "my_ndbm.h"
#include <stdint.h>


#define DEFAULT_BUFFER (1024)
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

#define ERROR_CREATE_USER_DUPLICATE_ALL (3)
#define ERROR_CREATE_USER_DUPLICATE_TOKEN (1)
#define ERROR_CREATE_USER_DUPLICATE_DISPLAY_NAME (2)

#define ERROR_DESTROY_USER_INVALID_FIELDS (1)
#define ERROR_DESTROY_USER_NOT_EXIST (2)
#define ERROR_DESTROY_USER_INVALID_CREDENTIALS (3)

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
#define SUCCESS_FORWARD_REQ (5)

#define ERROR_CREATE_CHANNEL_400 (400)
#define ERROR_CREATE_CHANNEL_403 (403)
#define ERROR_CREATE_CHANNEL_404 (404)
#define ERROR_CREATE_CHANNEL_409 (409)
#define ERROR_CREATE_CHANNEL_500 (500)

#define ERROR_READ_CHANNEL_500 (500)
#define ERROR_READ_CHANNEL_400 (400)
#define ERROR_READ_CHANNEL_404 (404)

#define ERROR_UPDATE_CHANNEL_500 (500)
#define ERROR_UPDATE_CHANNEL_400 (400)
#define ERROR_UPDATE_CHANNEL_404 (404)


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
    char dsply_name[DSPLY_NAME_LENGTH];
    char ip_address[CLNT_IP_ADDR_LENGTH];
    char login_token[TOKEN_NAME_LENGTH];
    char uuid[UUID_LEN];
    int privilege_level;
    time_t access_time;
} connected_user;


/* General functions */
int handle_request(int fd, const char* clnt_addr, connected_user* cache);
int read_header(int fd, chat_header_t *header_out);
uint32_t create_response_header(const chat_header_t* header);

/* related to USER */
int read_and_create_user(int fd, char token_out[TOKEN_NAME_LENGTH], uint16_t body_size);
int read_and_destroy_user(int fd, char token_out[TOKEN_NAME_LENGTH], uint16_t body_size, connected_user* cache);
int send_create_user_response(int fd, chat_header_t header, int result, const char* token, const char* clnt_addr);
int send_destroy_user_response(int fd, chat_header_t header, int result, const char* token, const char* clnt_addr);
user_login_t* generate_user_login_malloc_or_null(const char* login_token, const char* password, const char* user_id);
user_account_t* generate_user_account_malloc_or_null(const char* uuid, const char* display_name);

/* related to AUTH */
int read_and_login_user(int fd, char token_out[TOKEN_NAME_LENGTH], const char* clnt_addr, connected_user* cache);
char* build_channel_name_list(channel_info_t** channels, int channel_count);
int read_and_logout_user(int fd, char token_out[TOKEN_NAME_LENGTH], const char* clnt_addr, connected_user* cache);
int send_login_user_response(int fd, chat_header_t header, int result, const char* token, const char* clnt_addr);
int send_logout_user_response(int fd, chat_header_t header, int result, const char* token, const char* clnt_addr);
user_account_t* login_user_account_malloc_or_null(user_account_t* user_acc, const char* clnt_addr);
user_account_t* logout_user_account_malloc_or_null(user_account_t* user_acc);

/* related to CHANNEL */
int read_and_create_channel(int fd, char token_out[TOKEN_NAME_LENGTH], uint16_t body_size, const char clnt_addr[CLNT_IP_ADDR_LENGTH]);
int send_create_channel_response(int fd, chat_header_t header, int result, const char* token, const char* clnt_addr);
channel_info_t* create_channel_or_null_malloc(const char* channel_name, const char* display_name, bool publicity);
int read_and_read_channel(int fd, char* channel_info_out, uint16_t body_size);
int send_read_channel_response(int fd, chat_header_t header, int result, const char* channel_info_token, const char* clnt_addr);
int read_and_update_channel(int fd, char* channel_info_out, uint16_t body_size);
int send_update_channel_response(int fd, chat_header_t header, int result, const char* channel_info_token, const char* clnt_addr);

/* related to MESSAGE */
int read_and_create_message(int fd, char token_out[TOKEN_NAME_LENGTH], char forward_token[TOKEN_NAME_LENGTH], connected_user* cache);
int send_create_message_response(int fd, chat_header_t, int result, const char* token, const char* clnt_addr);
message_info_t* generate_message_malloc_or_null(char* display_name, channel_info_t* channel,
                                                char* message_body, const uint8_t* timestamp);

/* related to Cache, Util */
int get_num_connected_users(connected_user* cache);
void insert_user_in_cache(int fd, connected_user* cache, user_account_t* connecting_user, user_login_t* login_info, int num_active_users);
void remove_user_in_cache(connected_user* cache, user_account_t* connecting_user, int num_active_users);
int cmp_users(const void* a, const void* b);
int find_duplicate_user(connected_user* users, int n);
bool find_connected_user_with_same_cred(user_account_t* user_account, connected_user* conn_users, int num_users, int fd);
connected_user* get_connected_user_by_display_name(connected_user* cache, const char* display_name);
connected_user* get_connected_user_by_fd(connected_user* cache, int fd);
void view_active_users(connected_user* cache);
char* get_display_name_in_cache_malloc_or_null(const char ip_addr[CLNT_IP_ADDR_LENGTH]);


#endif //CHAT_SERVER_SERVER_H

