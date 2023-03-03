#ifndef CHAT_SERVER_LOGGER_H
#define CHAT_SERVER_LOGGER_H


/**
 * Writes a log of an event into a file.
 * @param filename a string represents a filename
 * @param ip_address a string represents a client's ip address
 * @param event_type a string represents an event type
 * @param i an integer represents the index of a client
 */
void log_event(const char* filename, char* ip_address, const char* event_type, int i);


#endif /* CHAT_SERVER_LOGGER_H */

