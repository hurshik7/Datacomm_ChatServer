#include "my_ndbm.h"
#include "server.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int handle_request(int fd)
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
                result = create_user(fd);
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

int create_user(int fd)
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

    user_login_t* login_info = get_login_info_malloc_or_null(token);
    if (login_info != NULL) {
        // the user already exist
        goto error_exit;
    }
    assert(login_info == NULL);

    strncpy(login_token, token, strlen(token));
    token = strtok(NULL, "\3"); // display-name
    strncpy(display_name, token, strlen(token));
    token = strtok(NULL, "\3"); // password
    strncpy(password, token, strlen(token));

    // else store the user in the db (login_info db, user_account db)
    // TODO Shik is working this part now.

    free(login_info);
    return 0;
error_exit:
    free(login_info);
    return ERROR_CREATE_USER_ALREADY_EXIST;
}

int send_response(int fd, int object, int type, int result)
{

    return 0;
}

