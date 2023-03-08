#include "my_ndbm.h"
#include "util.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


DBM* open_db_or_null(const char *db_name)
{
    DBM* db = dbm_open(db_name, O_CREAT | O_RDWR | O_SYNC | O_APPEND, 0644);
    if (!db) {
        perror("[DB]Error: Failed to create database.\n");
        return NULL;
    }
    return db;
}

user_login_t* get_login_info_malloc_or_null(char* login_token)
{
    DBM* login_info_db = open_db_or_null(DB_LOGIN_INFO);
    if (login_info_db == NULL) {
        perror("[DB]Error: Failed to open LOGIN_INFO DB");
        return NULL;
    }

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = login_token;
    key.dsize = strlen(login_token) + 1;

    value = dbm_fetch(login_info_db, key);
    if (value.dptr == NULL) {
        perror("[DB]Error: User not found.\n");
        return NULL;
    }

    user_login_t* login_info = (user_login_t*) malloc(sizeof(user_login_t));
    if (login_info == NULL) {
        perror("[DB]Error: malloc()");
        return NULL;
    }

    memcpy(login_info, value.dptr, sizeof(user_login_t));
    return login_info;
}

/*
void insertUser(DBM *db)
{
    struct User user;

    printf("Enter first name: ");
    scanf("%s", user.firstName);

    printf("Enter last name: ");
    scanf("%s", user.lastName);

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = user.firstName;
    key.dsize = strlen(user.firstName) + 1;
    value.dptr = (char *) &user;
    value.dsize = sizeof(struct User);

    if (dbm_store(db, key, value, DBM_REPLACE) != 0) {
        printf("Error: Failed to insert user.\n");
        exit(1);
    }

    printf("User inserted successfully.\n");
}

void fetchUser(DBM *db)
{
    char firstName[MAX_FIRST_NAME_LENGTH];

    printf("Enter first name: ");
    scanf("%s", firstName);

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = firstName;
    key.dsize = strlen(firstName) + 1;

    value = dbm_fetch(db, key);
    if (value.dptr == NULL) {
        printf("Error: User not found.\n");
    }

    struct User *user = (struct User *) value.dptr;

    printUser(*user);
}

void deleteUser(DBM *db)
{
    char firstName[MAX_FIRST_NAME_LENGTH];
    char lastName[MAX_LAST_NAME_LENGTH];

    printf("Enter the first name of the user to delete: ");
    scanf("%s", firstName);
    printf("Enter the last name of the user to delete: ");
    scanf("%s", lastName);

    datum key, value;
    memset(&key, 0, sizeof(datum));
    memset(&value, 0, sizeof(datum));

    key.dptr = firstName;
    key.dsize = strlen(firstName) + 1;
    value = dbm_fetch(db, key);
    if (value.dptr == NULL) {
        printf("Error: User not found.\n");
    }

    if (dbm_delete(db, key) == 0) {
        printf("Deleted user: %s\n", firstName);
    } else {
        printf("Error: Failed to delete record.\n");
    }
}

void printAllUsers(DBM *db) {
    datum key, data;
    key = dbm_firstkey(db);
    while (key.dptr != NULL) {
        data = dbm_fetch(db, key);
        if (data.dptr != NULL) {
            struct User *user = (struct User *)data.dptr;
            printf("First name: %s, Last name: %s\n", user->firstName, user->lastName);
        }
        key = dbm_nextkey(db);
    }
}

void optionHandler(DBM* db)
{
    char choice;
    while (1) {
        clean_stdin();
        printf("%s", "[0] - Insert\n"
                     "[1] - Fetch\n"
                     "[2] - Delete\n"
                     "[3] - Get Records\n"
                     "[x] - Exit\n");
        scanf("%c", &choice);
        switch (choice) {
            case '0':
                insertUser(db);
                break;
            case '1':
                fetchUser(db);
                break;
            case '2':
                deleteUser(db);
                break;
            case '3':
                printAllUsers(db);
                break;
            case 'X':
            case 'x':
                dbm_close(db);
                exit(0);
            default:
                printf("%s", "Invalid selection. Try again.\n");
                break;
        }
    }
}
*/