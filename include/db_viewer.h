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
// fcntl must be explicitly declared when running db_viewer on Linux
#include <fcntl.h>
#include <sys/utsname.h>

/**
 * db files stored here for macOS
 */
#define DB_VIEWER_LOGIN_INFO_LOC "../cmake-build-debug/user_login_info"
#define DB_VIEWER_DISPLAY_NAMES_LOC "../cmake-build-debug/display_names"
#define DB_VIEWER_USER_ACCOUNT_LOC "../cmake-build-debug/user_account_info"


void printAllUserAccounts(DBM *db);
void printAllDisplayNames(DBM *db);
void printAllLoginInfos(DBM *db);
void printAllChannelInfos(DBM *db);
void run_db_viewer(void);


#endif //CHAT_SERVER_DB_VIEWER_H

