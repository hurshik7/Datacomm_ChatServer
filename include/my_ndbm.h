#ifndef MY_NDBM_H
#define MY_NDBM_H


#include <ndbm.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <uuid/uuid.h>


#define MAX_FIRST_NAME_LENGTH 20
#define MAX_LAST_NAME_LENGTH 30
#define TOKEN_NAME_LENGTH (20)
#define PSWD_MAX_LENGTH (30)


#define DB_LOGIN_INFO "user_login_info"
#define DB_USER_ACCOUNT "user_account_info"


typedef struct UserLoginInfo {
    char login_token[TOKEN_NAME_LENGTH];
    char password[PSWD_MAX_LENGTH];
    uuid_t* id;
} user_login_t;

typedef struct UserAccountInfo {
    uuid_t* user_id;
    char display_name[TOKEN_NAME_LENGTH];
    struct sockaddr_in sock_addr;
    bool online_status;
    int privilege_level;
} user_account_t;


DBM* open_db_or_null(const char *db_name);
user_login_t* get_login_info_malloc_or_null(char *login_token);

/*
void insertUser(DBM *db);
void fetchUser(DBM *db);
void deleteUser(DBM *db);
void printAllUsers(DBM *db);
void optionHandler(DBM* db);
*/

#endif /* MY_NDBM_H */

