#ifndef CHAT_SERVER_SERVER_H
#define CHAT_SERVER_SERVER_H


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


typedef struct version_type {
    uint8_t  version:4;
    uint8_t  type:4;
} version_type_t;

typedef struct chat_header {
    version_type_t version_type;
    uint8_t        object;
    uint16_t       body_size;
} chat_header_t;


int handle_request(int fd);
int read_header(int fd, chat_header_t *header_out);



#endif //CHAT_SERVER_SERVER_H
