#ifndef CHAT_SERVER_DB_VIEWER_H
#define CHAT_SERVER_DB_VIEWER_H


#include "my_ndbm.h"
#include <ndbm.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/utsname.h>


void printAllUserAccounts(DBM *db);
void printAllDisplayNames(DBM *db);
void printAllLoginInfos(DBM *db);
void printAllChannelInfos(DBM *db);
void run_db_viewer(void);


#endif //CHAT_SERVER_DB_VIEWER_H

