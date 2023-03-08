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
        perror("read_header");
        return -1;
    }

    if (header.version_type.version != CURRENT_VERSION) {
        perror("wrong version");
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
                // TODO read body (info of the user)

                // TODO Find the user in the DB.
                    // if there is same user in the DB, send error response

                    // else store the user in the db

                // TODO send a response.
            } else if (header.version_type.type == TYPE_READ) {

            } else if (header.version_type.type == TYPE_UPDATE) {

            } else if (header.version_type.type == TYPE_DESTROY) {

            } else {
                perror("wrong type");
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
            perror("wrong object number");
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
        perror("read() in read_header()");
        return -1;
    }

    assert(nread == 4);
    int temp_header = 0;
    memcpy(&temp_header, buffer, HEADER_SIZE);
//    printf("temp_header before ntohl(): %d\n", temp_header);
    temp_header = ntohl(temp_header);
    memcpy(header_out, &temp_header, sizeof(chat_header_t));
    return 0;
}

