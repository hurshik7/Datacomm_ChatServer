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
    // do server base on request, read body if it needs to
    char token[TOKEN_NAME_LENGTH] = { '\0', };
    switch (header.object) {
        case OBJECT_USER:
            if (header.version_type.type == TYPE_CREATE) {
                result = read_and_create_user(fd, token);
                send_create_user_response(fd, header, result, token, clnt_addr);
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
    ssize_t nread = read(fd, &header_out->version_type, 1);
    if (nread < 0) {
        perror("[SERVER]Error: read() in read_header(), reading version_type");
        return -1;
    }

    nread = read(fd, &header_out->object, 1);
    if (nread < 0) {
        perror("[SERVER]Error: read() in read_header(), object");
        return -1;
    }

    nread = read(fd, &header_out->body_size, 2);
    if (nread < 0) {
        perror("[SERVER]Error: read() in read_header(), body_size");
        return -1;
    }

    header_out->body_size = ntohs(header_out->body_size);
    return 0;
}

int read_and_create_user(int fd, char token_out[TOKEN_NAME_LENGTH])
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
    char* token = strtok(buffer, "\3"); // login_token
    strncpy(login_token, token, strlen(token));
    token = strtok(NULL, "\3"); // display-name
    strncpy(display_name, token, strlen(token));

    bool is_token_duplicate = false;
    user_login_t* login_info = get_login_info_malloc_or_null(login_token);
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
    assert(login_info == NULL);

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
    strncpy(token_out, login_token, TOKEN_NAME_LENGTH);

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

int send_create_user_response(int fd, chat_header_t header, int result, const char* token, char* clnt_addr)
{
    char body[DEFUALT_BUFFER] = { '\0', };
    if (result == 0) {
        strcpy(body, "201\3\0");
        strcat(body, token);
    } else {
        sprintf(body, "409\3%d\3", result);
        if (result == ERROR_CREATE_USER_DUPLICATE_TOKEN) {
            strcat(body, "Login Token was not unique");
        } else if (result == ERROR_CREATE_USER_DUPLICATE_DISPLAY_NAME) {
            strcat(body, "Display Name was not unique");
        } else if (result == ERROR_CREATE_USER_DUPLICATE_ALL) {
            strcat(body, "Login Token and Display name both were not unique");
        } else {
            assert(!"should not be here");
        }
    }
    header.body_size = strlen(body);
    int header_int;
    memcpy(&header_int, &header, sizeof(chat_header_t));
    header_int = htonl(header_int);
    if (write(fd, &header_int, sizeof(chat_header_t)) < 0) {
        perror("send header (send_create_user_response)");
        return -1;
    }
    if (write(fd, body, header.body_size) < 0) {
        perror("send body (send_create_user_response)");
        return -1;
    }
    printf("Success to send the res to %s/res-body:%s\n", clnt_addr, body);
    return 0;
}

