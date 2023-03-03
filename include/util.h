#ifndef CHAT_SERVER_UTIL_H
#define CHAT_SERVER_UTIL_H


#include <arpa/inet.h>
#include <stdlib.h>


#define MAX_IP_ADD_STR_LENGTH (16)


/**
 * Stores ip address of a client into out_str
 * @param sa sockaddr struct
 * @param out_str a string that ip address will be stored
 */
void get_ip_str(const struct sockaddr *sa, char *out_str);

/**
 * Clear a string.
 * @param str a string
 * @param len the length of the string
 */
void clear_str(char *str, size_t len);


#endif /* CHAT_SERVER_UTIL_H */

