#include "my_ndbm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void printUser(user_t user)
{
    printf("First name: %s\n", user.firstName);
    printf("Last name: %s\n", user.lastName);
}

DBM* createDatabase(void)
{
    char fileName[30];
    printf("Enter database filename: ");
    scanf("%s", fileName);

    DBM *db = dbm_open(fileName, O_CREAT | O_RDWR | O_SYNC | O_APPEND, 0644);
    if (!db) {
        printf("Error: Failed to create database.\n");
        exit(1);
    }

    return db;
}

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

void optionHandler(DBM* db)
{
    char choice;

    while (1) {
        fflush(stdin);
        printf("%s", "[0] - Insert\n"
                     "[1] - Fetch\n"
                     "[x] - Exit\n");
        scanf("%c", &choice);
        switch (choice) {
            case '0':
                insertUser(db);
                break;
            case '1':
                fetchUser(db);
                break;
            case 'X':
            case 'x':
                dbm_close(db);
                exit(0);
            default:
                printf("%s", "Invalid selection. Try again.\n");
        }
    }
}
