#include <ndbm.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
// fcntl must be explicitly declared when running db_viewer on Linux
#include <fcntl.h>
#include <sys/utsname.h>


#define TOKEN_NAME_LENGTH (20)
#define PSWD_MAX_LENGTH (30)
#define UUID_LEN (37)
#define MAX_IP_ADD_STR_LENGTH (16)

/**
 * db files stored here for macOS
 */
#define DB_LOGIN_INFO "../cmake-build-debug/user_login_info"
#define DB_DISPLAY_NAMES "../cmake-build-debug/display_names"
#define DB_USER_ACCOUNT "../cmake-build-debug/user_account_info"

/**
 * db files stored here for Linux distros
 */
#define DB_LOGIN_INFO_LINUX "../src/user_login_info"
#define DB_DISPLAY_NAMES_LINUX "../src/display_names"
#define DB_USER_ACCOUNT_LINUX "../src/user_account_info"


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
struct utsname identify_os(void);

int main(void)
{
    char *user_acc = malloc(sizeof(char) * 256);
    char *user_dsply_nm = malloc(sizeof(char) * 256);
    char *user_log_info = malloc(sizeof(char) * 256);
    struct utsname my_data = identify_os();
    char* my_os = my_data.sysname;
    if (strcmp(my_os, "Darwin") == 0) {
        strncpy(user_acc, DB_USER_ACCOUNT, strlen(DB_USER_ACCOUNT));
        strncpy(user_dsply_nm, DB_DISPLAY_NAMES, sizeof(DB_DISPLAY_NAMES));
        strncpy(user_log_info, DB_LOGIN_INFO, sizeof(DB_LOGIN_INFO));
    } else if (strcmp(my_os, "Linux") == 0) {
        strncpy(user_acc, DB_USER_ACCOUNT_LINUX, sizeof(DB_USER_ACCOUNT_LINUX));
        strncpy(user_dsply_nm, DB_DISPLAY_NAMES_LINUX, sizeof(DB_DISPLAY_NAMES_LINUX));
        strncpy(user_log_info, DB_LOGIN_INFO_LINUX, sizeof(DB_LOGIN_INFO_LINUX));
    }
    puts("UserAccounts DB");
    DBM* user_accounts = dbm_open(user_acc, O_RDWR | O_SYNC | O_APPEND, 0644);
    printAllUserAccounts(user_accounts);
    puts("");

    puts("DisplayNames DB");
    DBM* display_names = dbm_open(user_dsply_nm, O_RDWR | O_SYNC | O_APPEND, 0644);
    printAllDisplayNames(display_names);
    puts("");

    puts("UserLoginInfo DB");
    DBM* login_infos = dbm_open(user_log_info, O_RDWR | O_SYNC | O_APPEND, 0644);
    printAllLoginInfos(login_infos);
    puts("");

    free(user_acc);
    free(user_dsply_nm);
    free(user_log_info);
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
            strncpy(clnt_addr, (const char*)&user->sock_addr, MAX_IP_ADD_STR_LENGTH);
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

struct utsname identify_os(void) {
    struct utsname unameData;
    int result = uname(&unameData);

    if (result == 0) {
        printf("Server is running on: %s\n", unameData.sysname);
    } else {
        printf("Error getting uname information\n");
    }

    return unameData;
}

