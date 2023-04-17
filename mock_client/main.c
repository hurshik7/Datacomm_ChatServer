#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <unistd.h>


#define DEFUALT_BUFFER (1024)
#define HEADER_SIZE (4)


typedef struct version_type {
    uint8_t  version:4;
    uint8_t  type:4;
} version_type_t;

typedef struct chat_header {
    version_type_t version_type;
    uint8_t        object;
    uint16_t       body_size;
} chat_header_t;


int copy(int from_fd, int to_fd, size_t count);
uint32_t create_response_header(const chat_header_t* header);


int main(void) {

    int client_socket;
    struct sockaddr_in server;

    // create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        printf("Could not create socket\n");
        return 1;
    }

    // prepare the sockaddr_in structure
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
//    server.sin_addr.s_addr = inet_addr("192.168.0.107");
    server.sin_family = AF_INET;
    server.sin_port = htons(5050);

    // connect to remote poll-server
    if (connect(client_socket, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }

    while (1) {
        char choice;

        printf("\n%s", "[0] - CREATE_ADMIN\n"
                     "[1] - CREATE_USER\n"
                     "[2] - CREATE_AUTH\n"
                     "[3] - DESTROY_AUTH\n"
                     "[4] - DESTROY_AUTH_ADMIN\n"
                     "[5] - CREATE_CHANNEL\n"
                     "[6] - CREATE_MESSAGE\n"
                     "[7] - DESTROY_USER\n"
                     "[8] - READ_CHANNEL\n"
                     "[9] - UPDATE_CHANNEL\n"
                     "[A] - READ_MESSAGE\n");
        scanf("%c", &choice);

        if (choice == '0') {
            // CREATE ADMIN
            chat_header_t test_header;
            memset(&test_header, 0, sizeof(chat_header_t));
            test_header.version_type.version = 1;
            test_header.version_type.type = 1;
            test_header.object = 1;
            // for testing read_header
            printf("version: %d\n", test_header.version_type.version);
            printf("type: %d\n", test_header.version_type.type);
            printf("object: %d\n", test_header.object);

            char body[] = "GlobalAdmin\3admin\3admin\0";
            test_header.body_size = (uint16_t) strlen(body);
            uint16_t body_size = test_header.body_size;
            test_header.body_size = htons(test_header.body_size);
            uint32_t temp_int = 0;
            memcpy(&temp_int, &test_header, sizeof(test_header));

                if (send(client_socket, &temp_int, sizeof(uint32_t), 0) < 0) {
                    perror("send");
                }

                if (send(client_socket, body, body_size, 0) < 0) {
                    perror("send");
                }

                char buffer[DEFUALT_BUFFER];
                if (read(client_socket, buffer, sizeof(test_header)) < 0) {
                    perror("read");
                }
                memset(buffer, '\0', DEFUALT_BUFFER);
                if (read(client_socket, buffer, DEFUALT_BUFFER) < 0) {
                    perror("recv");
                }
                printf("res: %s\n", buffer);
        } else if (choice == '1') {
            // CREATE USER
            chat_header_t test_header;
            memset(&test_header, 0, sizeof(chat_header_t));
            test_header.version_type.version = 1;
            test_header.version_type.type = 1;
            test_header.object = 1;
            // for testing read_header
            printf("version: %d\n", test_header.version_type.version);
            printf("type: %d\n", test_header.version_type.type);
            printf("object: %d\n", test_header.object);

            char body[] = "bennychao\3benny\3monkey123\0";
            test_header.body_size = (uint16_t) strlen(body);
            uint16_t body_size = test_header.body_size;
            test_header.body_size = htons(test_header.body_size);
            uint32_t temp_int = 0;
            memcpy(&temp_int, &test_header, sizeof(test_header));

                if (send(client_socket, &temp_int, sizeof(uint32_t), 0) < 0) {
                    perror("send");
                }

                if (send(client_socket, body, body_size, 0) < 0) {
                    perror("send");
                }

                char buffer[DEFUALT_BUFFER];
                if (read(client_socket, buffer, sizeof(test_header)) < 0) {
                    perror("read");
                }
                memset(buffer, '\0', DEFUALT_BUFFER);
                if (read(client_socket, buffer, DEFUALT_BUFFER) < 0) {
                    perror("recv");
                }
                printf("res: %s\n", buffer);
        } else if (choice == '2') {
            // CREATE_AUTH
            chat_header_t test_header;
            memset(&test_header, 0, sizeof(chat_header_t));
            test_header.version_type.version = 1;
            test_header.version_type.type = 1;
            test_header.object = 4;
            // for testing read_header
            printf("version: %d\n", test_header.version_type.version);
            printf("type: %d\n", test_header.version_type.type);
            printf("object: %d\n", test_header.object);

            char body[] = "bennychao\3monkey123\3\0";
            test_header.body_size = (uint16_t) strlen(body);
            uint16_t body_size = test_header.body_size;
            test_header.body_size = htons(test_header.body_size);

            uint32_t temp_int = 0;
            memcpy(&temp_int, &test_header, sizeof(test_header));

            if (send(client_socket, &temp_int, sizeof(uint32_t), 0) < 0) {
                perror("send");
            }

            if (send(client_socket, body, body_size, 0) < 0) {
                perror("send");
            }

            char buffer[DEFUALT_BUFFER];
            if (read(client_socket, buffer, sizeof(test_header)) < 0) {
                perror("read");
            }
            memset(buffer, '\0', DEFUALT_BUFFER);
            if (read(client_socket, buffer, DEFUALT_BUFFER) < 0) {
                perror("recv");
            }
            printf("res: %s\n", buffer);
            sleep(1);
        } else if (choice == '3') {
            // DESTROY_AUTH
            chat_header_t test_header;
            memset(&test_header, 0, sizeof(chat_header_t));
            test_header.version_type.version = (uint8_t) 1;
            test_header.version_type.type = (uint8_t) 4;
            test_header.object = (uint8_t) 4;
            // for testing read_header
            printf("version: %d\n", test_header.version_type.version);
            printf("type: %d\n", test_header.version_type.type);
            printf("object: %d\n", test_header.object);

            char body[] = "benny\3\0";
            test_header.body_size = (uint16_t) strlen(body);
            uint16_t body_size = test_header.body_size;

            uint32_t temp_int = create_response_header(&test_header);
            if (send(client_socket, &temp_int, sizeof(uint32_t), 0) < 0) {
                perror("send");
            }

            if (send(client_socket, body, body_size, 0) < 0) {
                perror("send");
            }

            char buffer[DEFUALT_BUFFER];
            if (read(client_socket, buffer, sizeof(test_header)) < 0) {
                perror("read");
            }
            memset(buffer, '\0', DEFUALT_BUFFER);
            if (read(client_socket, buffer, DEFUALT_BUFFER) < 0) {
                perror("recv");
            }
            printf("res: %s\n", buffer);
            sleep(1);
        } else if (choice == '4') {
            // DESTROY_AUTH_ADMIN
            chat_header_t test_header;
            memset(&test_header, 0, sizeof(chat_header_t));
            test_header.version_type.version = 1;
            test_header.version_type.type = 4;
            test_header.object = 4;
            // for testing read_header
            printf("version: %d\n", test_header.version_type.version);
            printf("type: %d\n", test_header.version_type.type);
            printf("object: %d\n", test_header.object);

            char body[] = "bennychao\0";
            test_header.body_size = (uint16_t) strlen(body);
            uint16_t body_size = test_header.body_size;
            test_header.body_size = htons(test_header.body_size);

            uint32_t temp_int = 0;
            memcpy(&temp_int, &test_header, sizeof(test_header));

            if (send(client_socket, &temp_int, sizeof(uint32_t), 0) < 0) {
                perror("send");
            }

            if (send(client_socket, body, body_size, 0) < 0) {
                perror("send");
            }

            char buffer[DEFUALT_BUFFER];
            if (read(client_socket, buffer, sizeof(test_header)) < 0) {
                perror("read");
            }
            memset(buffer, '\0', DEFUALT_BUFFER);
            if (read(client_socket, buffer, DEFUALT_BUFFER) < 0) {
                perror("recv");
            }
            printf("res: %s\n", buffer);
        } else if (choice == '5') {
            // CREATE CHANNEL
            chat_header_t test_header;
            memset(&test_header, 0, sizeof(chat_header_t));
            test_header.version_type.version = 1;
            test_header.version_type.type = 1;
            test_header.object = 2;
            // for testing read_header
            printf("version: %d\n", test_header.version_type.version);
            printf("type: %d\n", test_header.version_type.type);
            printf("object: %d\n", test_header.object);

            char body[1024] = {'\0'};
            sprintf(body, "comp4981 channel\3benny\3%d", 0);
            test_header.body_size = (uint16_t) strlen(body);
            uint16_t body_size = test_header.body_size;
            test_header.body_size = htons(test_header.body_size);

            uint32_t temp_int = 0;
            memcpy(&temp_int, &test_header, sizeof(test_header));

            if (send(client_socket, &temp_int, sizeof(uint32_t), 0) < 0) {
                perror("send");
            }

            if (send(client_socket, body, body_size, 0) < 0) {
                perror("send");
            }

            char buffer[DEFUALT_BUFFER];
            if (read(client_socket, buffer, sizeof(uint32_t)) < 0) {
                perror("read");
            }
            memset(buffer, '\0', DEFUALT_BUFFER);
            if (read(client_socket, buffer, DEFUALT_BUFFER) < 0) {
                perror("recv");
            }
            printf("res-body: %s\n", buffer);
        } else if (choice == '6') {
            // CREATE MESSAGE
            chat_header_t test_header;
            memset(&test_header, 0, sizeof(chat_header_t));
            test_header.version_type.version = 1;
            test_header.version_type.type = 1;
            test_header.object = 3;
            // for testing read_header
            printf("version: %d\n", test_header.version_type.version);
            printf("type: %d\n", test_header.version_type.type);
            printf("object: %d\n", test_header.object);

            time_t send_time = time(NULL);
            uint8_t send_this = send_time;
            printf("%ld", send_time);

            char body[1024] = {'\0'};
            sprintf(body, "benny\3comp4981 channel\3new message\3%hhu", send_this);
            test_header.body_size = (uint16_t) strlen(body);
            uint16_t body_size = test_header.body_size;

            uint32_t temp_int = create_response_header(&test_header);

            if (send(client_socket, &temp_int, sizeof(chat_header_t), 0) < 0) {
                perror("send");
            }

            if (send(client_socket, body, body_size, 0) < 0) {
                perror("send");
            }

            char buffer[DEFUALT_BUFFER];
            if (read(client_socket, buffer, sizeof(uint32_t)) < 0) {
                perror("read");
            }
            memset(buffer, '\0', DEFUALT_BUFFER);
            if (read(client_socket, buffer, DEFUALT_BUFFER) < 0) {
                perror("recv");
            }
            printf("res-body: %s\n", buffer);
        } else if (choice == '7') {
            // DESTROY USER

            chat_header_t test_header;
            memset(&test_header, 0, sizeof(chat_header_t));
            test_header.version_type.version = 1;
            test_header.version_type.type = 4;
            test_header.object = 1;
            // for testing read_header
            printf("version: %d\n", test_header.version_type.version);
            printf("type: %d\n", test_header.version_type.type);
            printf("object: %d\n", test_header.object);

            char body[1024] = {'\0'};
            sprintf(body, "benny\3bennychao\3monkey123");
            test_header.body_size = (uint16_t) strlen(body);
            uint16_t body_size = test_header.body_size;

            uint32_t temp_int = create_response_header(&test_header);

            if (send(client_socket, &temp_int, sizeof(chat_header_t), 0) < 0) {
                perror("send");
            }

            if (send(client_socket, body, body_size, 0) < 0) {
                perror("send");
            }

            char buffer[DEFUALT_BUFFER];
            if (read(client_socket, buffer, sizeof(uint32_t)) < 0) {
                perror("read");
            }
            memset(buffer, '\0', DEFUALT_BUFFER);
            if (read(client_socket, buffer, DEFUALT_BUFFER) < 0) {
                perror("recv");
            }
            printf("res-body: %s\n", buffer);
        } else if (choice == '8') {
            // READ CHANNEL

            chat_header_t test_header;
            memset(&test_header, 0, sizeof(chat_header_t));
            test_header.version_type.version = (uint8_t) 1;
            test_header.version_type.type = (uint8_t) 2;
            test_header.object = (uint8_t) 2;
            // for testing read_header
            printf("version: %d\n", test_header.version_type.version);
            printf("type: %d\n", test_header.version_type.type);
            printf("object: %d\n", test_header.object);

            char body[1024] = {'\0'};
            sprintf(body, "comp4981 channel\3");
            strcat(body, "1\3"); // get users in the channel
            strcat(body, "1\3"); // get admin list
            strcat(body, "0\3"); // don't want to get the banned list
            test_header.body_size = (uint16_t) strlen(body);
            uint16_t body_size = test_header.body_size;

            uint32_t temp_int = create_response_header(&test_header);
            printf("sizeof(chat_header): %ld, sizeof(uint32_t): %ld\n", sizeof(chat_header_t), sizeof(uint32_t));

            if (send(client_socket, &temp_int, sizeof(chat_header_t), 0) < 0) {
                perror("send");
            }

            if (send(client_socket, body, body_size, 0) < 0) {
                perror("send");
            }

            char buffer[DEFUALT_BUFFER];
            if (read(client_socket, buffer, sizeof(uint32_t)) < 0) {
                perror("read");
            }
            uint32_t h = 0;
            memcpy(&h, buffer, sizeof(uint32_t));
            h = ntohl(h);
            chat_header_t temp_header;
            memcpy(&temp_header, &h, sizeof(uint32_t));

            memset(buffer, '\0', DEFUALT_BUFFER);
            if (read(client_socket, buffer, temp_header.body_size) < 0) {
                perror("recv");
            }
            printf("res-body: %s\n", buffer);
        } else if (choice == '9') {
            // UPDATE CHANNEL

            chat_header_t test_header;
            memset(&test_header, 0, sizeof(chat_header_t));
            test_header.version_type.version = 1;
            test_header.version_type.type = 3;
            test_header.object = 2;
            // for testing read_header
            printf("version: %d\n", test_header.version_type.version);
            printf("type: %d\n", test_header.version_type.type);
            printf("object: %d\n", test_header.object);

            char body[1024] = {'\0'};
            sprintf(body, "comp4981 channel\3");
            strcat(body, "0\3"); // change channel name?
            strcat(body, "0\3"); // don't want to get the banned list
            strcat(body, "1\3"); // alter users?
            strcat(body, "1\3admin\3"); // users to add
            strcat(body, "0\3"); // alter admins?
            strcat(body, "0\3"); // alter banned?
            test_header.body_size = (uint16_t) strlen(body);
            uint16_t body_size = test_header.body_size;

            uint32_t temp_int = create_response_header(&test_header);

            if (send(client_socket, &temp_int, sizeof(chat_header_t), 0) < 0) {
                perror("send");
            }

            if (send(client_socket, body, body_size, 0) < 0) {
                perror("send");
            }

            char buffer[DEFUALT_BUFFER];
            if (read(client_socket, buffer, sizeof(uint32_t)) < 0) {
                perror("read");
            }
            uint32_t h = 0;
            memcpy(&h, buffer, sizeof(uint32_t));
            h = ntohl(h);
            chat_header_t temp_header;
            memcpy(&temp_header, &h, sizeof(uint32_t));

            memset(buffer, '\0', DEFUALT_BUFFER);
            if (read(client_socket, buffer, temp_header.body_size) < 0) {
                perror("recv");
            }
            printf("res-body: %s\n", buffer);
        } else if (choice == 'A') {
            // READ MESSAGE

            chat_header_t test_header;
            memset(&test_header, 0, sizeof(chat_header_t));
            test_header.version_type.version = 1;
            test_header.version_type.type = 2;
            test_header.object = 3;
            // for testing read_header
            printf("version: %d\n", test_header.version_type.version);
            printf("type: %d\n", test_header.version_type.type);
            printf("object: %d\n", test_header.object);

            char body[1024] = {'\0'};
            sprintf(body, "comp4981 channel\3%d\3", 10);
            test_header.body_size = (uint16_t) strlen(body);
            uint16_t body_size = test_header.body_size;

            uint32_t temp_int = create_response_header(&test_header);

            if (send(client_socket, &temp_int, sizeof(chat_header_t), 0) < 0) {
                perror("send");
            }

            if (send(client_socket, body, body_size, 0) < 0) {
                perror("send");
            }

            char buffer[DEFUALT_BUFFER];
            if (read(client_socket, buffer, sizeof(uint32_t)) < 0) {
                perror("read");
            }
            uint32_t h = 0;
            memcpy(&h, buffer, sizeof(uint32_t));
            h = ntohl(h);
            chat_header_t temp_header;
            memcpy(&temp_header, &h, sizeof(uint32_t));

            memset(buffer, '\0', DEFUALT_BUFFER);
            if (read(client_socket, buffer, temp_header.body_size) < 0) {
                perror("recv");
            }
            printf("res-body: %s\n", buffer);
        }
    }

    // close the file and the socket.
    // close(client_socket);
    return 0;
}

int copy(int from_fd, int to_fd, size_t count)   // NOLINT(bugprone-easily-swappable-parameters)
{
    char *buffer;
    ssize_t rbytes;
    size_t total_rbytes = 0;

    buffer = (char*) malloc(count);

    if(buffer == NULL)
    {
        return EXIT_FAILURE;
    }

    while((rbytes = read(from_fd, buffer, count)) > 0)
    {
        ssize_t wbytes;

        wbytes = write(to_fd, buffer, rbytes);
        total_rbytes += wbytes;

        if(wbytes == -1)
        {
            free(buffer);
            return EXIT_FAILURE;
        }

        if (total_rbytes >= count)
        {
            break;
        }
    }

    free(buffer);
    if(rbytes == -1)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

uint32_t create_response_header(const chat_header_t* header)
{
    uint32_t serialized_header;
    uint32_t version;
    uint32_t type;
    uint32_t object;
    uint32_t body_size;

    version = header->version_type.version;
    type = header->version_type.type;
    object = header->object;
    body_size = header->body_size;

    version <<= 28;
    type <<= 24;
    object <<= 16;

    serialized_header = (version | type | object | body_size);

    serialized_header = htonl(serialized_header);
    return serialized_header;
}
