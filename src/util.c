#include "util.h"
#include <string.h>
#include <stdio.h>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
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

void clear_str(char *str, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++) {
        str[i] = '\0';
    }
}

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}
