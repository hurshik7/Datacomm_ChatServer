#include "my_ndbm.h"
#include "server.h"
#include "util.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int handle_request(int fd, const char* clnt_addr)
{
    int result;
    chat_header_t header;
    memset(&header, 0, sizeof(header));
    result = read_header(fd, &header);
    if (result < 0) {
        perror("[SERVER]Error: read_header()");
        return -1;
    }

    if (header.version_type.version != CURRENT_VERSION) {
        perror("[SERVER]Error: wrong version");
        return ERROR_WRONG_VERSION;
    }
    /*
    {
        // for testing read_header
        printf("version: %d\n", header.version_type.version);
        printf("type: %d\n", header.version_type.type);
        printf("object: %d\n", header.object);
        printf("body_size: %d\n", header.body_size);
    }
     */

    assert(header.version_type.version == CURRENT_VERSION);
    // TODO do server based on request, read body if it needs to
    char body_buffer[DEFUALT_BUFFER];
    memset(body_buffer, '\0', sizeof(body_buffer));
    switch (header.object) {
        case OBJECT_USER:
            if (header.version_type.type == TYPE_CREATE) {
                result = read_and_create_user(fd);
                send_response(fd, OBJECT_USER, TYPE_CREATE, result);
            } else if (header.version_type.type == TYPE_READ) {

            } else if (header.version_type.type == TYPE_UPDATE) {

            } else if (header.version_type.type == TYPE_DESTROY) {

            } else {
                perror("[SERVER]Error: wrong type");
                assert(!"should not be here");
            }
            break;
        case OBJECT_CHANNEL:

            break;
        case OBJECT_MESSAGE:

            break;
        case OBJECT_AUTH:
            // it relates to login(CREATE), logout(DESTROY)

            break;
        default:
            perror("[SERVER]Error: wrong object number");
            assert(!"This should not be here");
    }

    return 0;
}

int read_header(int fd, chat_header_t *header_out)
{
    char buffer[DEFUALT_BUFFER];
    memset(buffer, 0, DEFUALT_BUFFER);
    ssize_t nread = read(fd, buffer, HEADER_SIZE);
    if (nread < 0) {
        perror("[SERVER]Error: read() in read_header()");
        return -1;
    }

    assert(nread == 4);
    int temp_header = 0;
    memcpy(&temp_header, buffer, HEADER_SIZE);
    temp_header = ntohl(temp_header);
    memcpy(header_out, &temp_header, sizeof(chat_header_t));
    return 0;
}

int read_and_create_user(int fd)
{
    char buffer[DEFUALT_BUFFER];
    memset(buffer, '\0', DEFUALT_BUFFER);
    ssize_t nread = read(fd, buffer, DEFUALT_BUFFER);
    if (nread <= 0) {
        perror("[SERVER]Error: read body");
        return -1;
    }

    char login_token[TOKEN_NAME_LENGTH] = { '\0', };
    char display_name[TOKEN_NAME_LENGTH] = { '\0', };
    char password[PSWD_MAX_LENGTH] = { '\0', };
    char* token = strtok(buffer, "\3");

    bool is_token_duplicate = false;
    user_login_t* login_info = get_login_info_malloc_or_null(token);
    bool is_display_name_duplicates = check_duplicate_display_name(display_name);
    if (login_info != NULL) {
        // the user already exist
        is_token_duplicate = true;
    }

    if (is_token_duplicate == true && is_display_name_duplicates == true) {
        goto error_exit_duplicate_token_and_name;
    }
    if (is_token_duplicate == true) {
        goto error_exit_duplicate_login_token;
    }
    if (is_display_name_duplicates == true) {
        goto error_exit_duplicate_display_name;
    }
    assert(is_token_duplicate == false && is_display_name_duplicates == false);
    assert(login_info != NULL);

    strncpy(login_token, token, strlen(token));
    token = strtok(NULL, "\3"); // display-name
    strncpy(display_name, token, strlen(token));
    token = strtok(NULL, "\3"); // password
    strncpy(password, token, strlen(token));

    //  store the user in the db (login_info db, user_account db, and also display_name db)
    char* user_uuid_malloc = generate_random_uuid_malloc();
    if (user_uuid_malloc != NULL) {
        insert_display_name(display_name, user_uuid_malloc);
    }
    user_login_t* user_login = generate_user_login_malloc_or_null(login_token, password, user_uuid_malloc);
    if (user_login != NULL) {
        insert_user_login(user_login);
    }
    user_account_t* user_account = generate_user_account_malloc_or_null(user_uuid_malloc, display_name);
    if (user_account != NULL) {
        insert_user_account(user_account);
    }

    free(user_account);
    free(user_login);
    free(user_uuid_malloc);
    free(login_info);
    return 0;
error_exit_duplicate_token_and_name:
    free(login_info);
    return ERROR_CREATE_USER_DUPLICATE_ALL;
error_exit_duplicate_login_token:
    free(login_info);
    return ERROR_CREATE_USER_DUPLICATE_TOKEN;
error_exit_duplicate_display_name:
    free(login_info);
    return ERROR_CREATE_USER_DUPLICATE_DISPLAY_NAME;
}

user_login_t* generate_user_login_malloc_or_null(const char* login_token, const char* password, const char* user_id)
{
    user_login_t* user_login = (user_login_t*) malloc(sizeof(user_login_t));
    if (user_login == NULL) {
        perror("generate user_login_t");
        return NULL;
    }
    memset(user_login, 0, sizeof(user_login_t));
    strncpy(user_login->login_token, login_token, TOKEN_NAME_LENGTH);
    strncpy(user_login->password, password, PSWD_MAX_LENGTH);
    strncpy(user_login->uuid, user_id, UUID_LEN);
    return user_login;
}

user_account_t* generate_user_account_malloc_or_null(const char* uuid, const char* display_name)
{
    user_account_t* user_account = (user_account_t*) malloc(sizeof(user_account_t));
    if (user_account == NULL) {
        perror("generate user_account_t");
        return NULL;
    }
    memset(user_account, 0, sizeof(user_account_t));
    strncpy(user_account->user_id, uuid, UUID_LEN);
    strncpy(user_account->display_name, display_name, TOKEN_NAME_LENGTH);
    return user_account;
}

int send_response(int fd, int object, int type, int result)
{

    return 0;
}

