#ifndef CHAT_SERVER_UTIL_H
#define CHAT_SERVER_UTIL_H


#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>


#define MAX_IP_ADD_STR_LENGTH (16)
#define DEFAULT_ARGS (10)


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

/**
 * Clean standard input stream.
 */
void clean_stdin(void);

/**
 * Tokenize a string. It uses malloc so that the caller should free it.
 * @param str a string to tokenize
 * @param delim a string contains delimiters
 * @param out_count a pointer to unsigned integer which will store the number of tokens
 * @return an array of strings contains all tokens
 */
char** tokenize_malloc(const char* str, const char* delim, uint32_t* out_count);

/**
 * Generate random UUID.
 * @return a string which is dynamically allocated
 */
char* generate_random_uuid_malloc(void);

/**
 * Compare two strings.
 * @param str1 a string
 * @param str2 a string
 * @return a boolean value
 */
bool compare_strings(const char* str1, const char* str2);

/**
 * Identifies the Operating System of the host running the server.
 * @return struct utsname contains os name, release level, ver. level, etc.
 */
struct utsname identify_os(void);

/**
 * Check if user input is a number.
 * @param str input string
 * @return true if input is a number else false
 */
bool is_number(const char *str);

#endif /* CHAT_SERVER_UTIL_H */
