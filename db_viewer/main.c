#include <ndbm.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
// fcntl must be explicitly declared when running db_viewer on Linux
#include <fcntl.h>


#define TOKEN_NAME_LENGTH (20)
#define PSWD_MAX_LENGTH (30)
#define UUID_LEN (37)
#define MAX_IP_ADD_STR_LENGTH (16)

/**
 * db files stored here for macOS
 */
//#define DB_LOGIN_INFO "../cmake-build-debug/user_login_info"
//#define DB_DISPLAY_NAMES "../cmake-build-debug/display_names"
//#define DB_USER_ACCOUNT "../cmake-build-debug/user_account_info"

/**
 * db files stored here for Linux distros
 */
#define DB_LOGIN_INFO "../src/user_login_info"
#define DB_DISPLAY_NAMES "../src/display_names"
#define DB_USER_ACCOUNT "../src/user_account_info"


typedef struct UserLoginInfo {
    char login_token[TOKEN_NAME_LENGTH];
    char password[PSWD_MAX_LENGTH];
    char uuid[UUID_LEN];
} user_login_t;

typedef struct UserAccountInfo {
    char user_id[UUID_LEN];
    char display_name[TOKEN_NAME_LENGTH];
    struct sockaddr_in sock_addr;
    bool online_status;
    int privilege_level;
} user_account_t;

void printAllUserAccounts(DBM *db);
void printAllDisplayNames(DBM *db);
void printAllLoginInfos(DBM *db);
void get_ip_str(const struct sockaddr *sa, char *out_str);

int main(int argc, char* argv[])
{
    puts("UserAccounts DB");
    DBM* user_accounts = dbm_open(DB_USER_ACCOUNT, O_RDWR | O_SYNC | O_APPEND, 0644);
    printAllUserAccounts(user_accounts);
    puts("");

    puts("DisplayNames DB");
    DBM* display_names = dbm_open(DB_DISPLAY_NAMES, O_RDWR | O_SYNC | O_APPEND, 0644);
    printAllDisplayNames(display_names);
    puts("");

    puts("UserLoginInfo DB");
    DBM* login_infos = dbm_open(DB_LOGIN_INFO, O_RDWR | O_SYNC | O_APPEND, 0644);
    printAllLoginInfos(login_infos);
    puts("");

    dbm_close(login_infos);
    dbm_close(display_names);
    dbm_close(user_accounts);
    return EXIT_SUCCESS;
}

void printAllUserAccounts(DBM *db)
{
    datum key, data;
    key = dbm_firstkey(db);
    while (key.dptr != NULL) {
        data = dbm_fetch(db, key);
        if (data.dptr != NULL) {
            struct UserAccountInfo* user = (struct UserAccountInfo *)data.dptr;
            char* clnt_addr = malloc(20);
            strncpy(clnt_addr, (const char*)&user->sock_addr.sin_addr, MAX_IP_ADD_STR_LENGTH);
            printf("%s, %s, %s, %d, %d\n", user->user_id, user->display_name,
                   clnt_addr, user->online_status, user->privilege_level);
        }
        key = dbm_nextkey(db);
    }
}

void printAllDisplayNames(DBM *db)
{
    datum key, data;
    key = dbm_firstkey(db);
    while (key.dptr != NULL) {
        data = dbm_fetch(db, key);
        if (data.dptr != NULL) {
            char* user_uuid = (char*) data.dptr;
            printf("%s, %s\n", (char*) key.dptr, user_uuid);
        }
        key = dbm_nextkey(db);
    }
}

void printAllLoginInfos(DBM *db)
{
    datum key, data;
    key = dbm_firstkey(db);
    while (key.dptr != NULL) {
        data = dbm_fetch(db, key);
        if (data.dptr != NULL) {
            user_login_t* user = (user_login_t*) data.dptr;
            printf("%s, %s, %s\n", user->login_token, user->password, user->uuid);
        }
        key = dbm_nextkey(db);
    }
}

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

