#ifndef NDBM_NDBM_H
#define NDBM_NDBM_H


#include <ndbm.h>


#define MAX_FIRST_NAME_LENGTH 20
#define MAX_LAST_NAME_LENGTH 30


typedef struct User {
    char firstName[MAX_FIRST_NAME_LENGTH];
    char lastName[MAX_LAST_NAME_LENGTH];
} user_t;

void printUser(struct User user);
DBM* createDatabase(void);
void insertUser(DBM *db);
void fetchUser(DBM *db);
void deleteUser(DBM *db);
void optionHandler(DBM* db);

#endif //NDBM_NDBM_H
