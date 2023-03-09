#ifndef CHAT_SERVER_SERVER_H
#define CHAT_SERVER_SERVER_H


#include "my_ndbm.h"
#include <stdint.h>


#define DEFUALT_BUFFER (1024)
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


typedef struct version_type {
    uint8_t  version:4;
    uint8_t  type:4;
} version_type_t;

typedef struct chat_header {
    version_type_t version_type;
    uint8_t        object;
    uint16_t       body_size;
} chat_header_t;


int handle_request(int fd, const char* clnt_addr);
int read_header(int fd, chat_header_t *header_out);
int read_and_create_user(int fd, char token_out[TOKEN_NAME_LENGTH]);
user_login_t* generate_user_login_malloc_or_null(const char* login_token, const char* password, const char* user_id);
user_account_t* generate_user_account_malloc_or_null(const char* uuid, const char* display_name);
int send_create_user_response(int fd, chat_header_t header, int result, const char* token, char* clnt_addr);


#endif //CHAT_SERVER_SERVER_H
