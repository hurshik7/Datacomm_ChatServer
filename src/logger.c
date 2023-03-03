#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_CLIENTS (100)

void log_event(const char* filename, char* ip_address, const char* event_type, int i) {
    time_t current_time;
    char* c_time_string;
    static time_t connection_time[MAX_CLIENTS];

    // Get the current time
    current_time = time(NULL);
    c_time_string = ctime(&current_time);
    c_time_string[strlen(c_time_string) - 1] = '\0';

    // Open the log file for appending
    FILE* log_file = fopen(filename, "a");

    if (strcmp(event_type, "CONNECT") == 0) {
        // Store the connection time
        connection_time[i] = current_time;

        // Write the connection event to the log file
        fprintf(log_file, "%s, %s, %s\n", c_time_string, event_type, ip_address);
    } else if (strcmp(event_type, "DISCONNECT") == 0) {
        // Calculate the duration of the connection in milliseconds
        long long duration = (long long) (difftime(current_time, connection_time[i]) * 1000);

        // Write the disconnection event with the duration to the log file
        fprintf(log_file, "%s, %s, %s, %lld\n", c_time_string, event_type, ip_address, duration);
    } else {
        // Write the event to the log file
        fprintf(log_file, "%s, %s, %s\n", c_time_string, event_type, ip_address);
    }

    // Close the log file
    fclose(log_file);
}
