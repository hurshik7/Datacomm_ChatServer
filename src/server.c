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

    {
        // for testing read_header
        printf("version: %d\n", header.version_type.version);
        printf("type: %d\n", header.version_type.type);
        printf("object: %d\n", header.object);
        printf("body_size: %d\n", header.body_size);
    }

    // TODO read body

    // TODO do server based on request

    // TODO send response

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
    printf("temp_header before ntohl(): %d\n", temp_header);
    temp_header = ntohl(temp_header);
    memcpy(header_out, &temp_header, sizeof(chat_header_t));
    return 0;
}
