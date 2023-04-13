#ifndef CHAT_SERVER_DB_VIEWER_H
#define CHAT_SERVER_DB_VIEWER_H


#include "my_ndbm.h"


/**
 * Prints all user accounts.
 * @param db a DBM pointer
 */
void printAllUserAccounts(DBM *db);

/**
 * Prints all display names.
 * @param db a DBM pointer
 */
void printAllDisplayNames(DBM *db);

/**
 * Prints all login information.
 * @param db a DBM pointer
 */
void printAllLoginInfos(DBM *db);

/**
 * Prints all channels.
 * @param db a DBM pointer
 */
void printAllChannelInfos(DBM *db);

/**
 * Runs this DB viewer.
 */
void run_db_viewer(void);


#endif //CHAT_SERVER_DB_VIEWER_H

