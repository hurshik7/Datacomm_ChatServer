#include "util.h"
#include <string.h>
#include <stdio.h>
#include <uuid/uuid.h>
#include <ctype.h>
#include <sys/utsname.h>


#define UUID_LEN (37)


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
/**
 * Retrieves IP as a string
 * @param sa a pointer to sockaddr struct
 * @param out_str a pointer to character buffer
 */
void get_ip_str(const struct sockaddr *sa, char *out_str)
{
    switch(sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((const struct sockaddr_in*)sa)->sin_addr), out_str, MAX_IP_ADD_STR_LENGTH);  // NOLINT(clang-diagnostic-cast-align)
            break;
        case AF_INET6:
            inet_ntop(AF_INET6, &(((const struct sockaddr_in6 *)sa)->sin6_addr), out_str, MAX_IP_ADD_STR_LENGTH); // NOLINT(clang-diagnostic-cast-align)
            break;
        default:
            strncpy(out_str, "Unknown AF", MAX_IP_ADD_STR_LENGTH);
            out_str[MAX_IP_ADD_STR_LENGTH - 1] = '\0';
    }
}
#pragma GCC diagnostic pop

/**
 * Clears the string
 * @param str a string
 * @param len length of string
 */
void clear_str(char *str, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++) {
        str[i] = '\0';
    }
}

/**
 * Clears standard input.
 */
void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

/**
 * Tokenizes the input string.
 * @param str a pointer to a string
 * @param delim a pointer that contains the delimiter characters
 * @param out_count a pointer the an unsigned 32 bit integer
 * @return pointer to ret.
 */
char** tokenize_malloc(const char* str, const char* delim, uint32_t* out_count)
{
    char* temp_str;
    char* token;
    size_t str_length = strlen(str);
    size_t word_count = 0;
    size_t ret_size = DEFAULT_ARGS;
    size_t word_length;
    char** ret;
    void* temp = NULL;

    temp_str = malloc((str_length + 1) * sizeof(char));
    strcpy(temp_str, str);

    ret = malloc(ret_size * sizeof(char*));

    token = strtok(temp_str, delim);                            // NOLINT(concurrency-mt-unsafe)
    if (token == NULL) {
        ret[0] = NULL;
        goto free_and_exit;
    }

    while (token != NULL) {
        word_count++;
        word_length = strlen(token);

        if (word_count == ret_size - 1) {
            ret_size += ret_size;
            temp = realloc(ret, ret_size * sizeof(char*));
            if (temp != NULL) {
                ret = temp;
            }
        }
        ret[word_count - 1] = malloc((word_length + 1) * sizeof(char));
        strcpy(ret[word_count - 1], token);
        if (ret[word_count - 1][strlen(ret[word_count - 1]) - 1] == '\n') {
            ret[word_count - 1][strlen(ret[word_count - 1]) - 1] = '\0';
        }

        token = strtok(NULL, delim);                            // NOLINT(concurrency-mt-unsafe)
    }

    ret[word_count] = NULL;
    *out_count = word_count;

    free_and_exit:
    free(temp_str);
    temp_str = NULL;
    return ret;
}

/**
 * Generates a random user id.
 * @return uuid or NULL.
 */
char* generate_random_uuid_malloc(void)
{
    uuid_t bin_uuid;
    uuid_generate_random(bin_uuid);
    char* uuid = (char*) malloc(UUID_LEN);
    if (uuid == NULL) {
        return NULL;
    }

    uuid_unparse_upper(bin_uuid, uuid);
    return uuid;
}

/**
 * Compares strings
 * @param str1 a string
 * @param str2 a string
 * @return true if match or false if no match.
 */
bool compare_strings(const char* str1, const char* str2)
{
    int i = 0, j = 0;
    while (str1[i] != '\0' && str2[j] != '\0') {
        if (isspace(str1[i])) {
            i++;
            continue;
        }
        if (isspace(str2[j])) {
            j++;
            continue;
        }
        if (str1[i] != str2[j]) {
            return false;
        }
        i++;
        j++;
    }
    while (isspace(str1[i])) {
        i++;
    }
    while (isspace(str2[j])) {
        j++;
    }
    return str1[i] == '\0' && str2[j] == '\0';
}

/**
 * Identifies operating system.
 * @return unameData.
 */
struct utsname identify_os(void)
{
    struct utsname unameData;
    int result = uname(&unameData);
    if (result < 0) {
        perror("unname()");
    }
    return unameData;
}

/**
 * Checks for a number.
 * @param str a string
 * @return True if number, False if it isn't.
 */
bool is_number(const char *str)
{
    for (int i = 0; str[i] != '\0'; ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}
