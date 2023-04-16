#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Print error message with an error code and exit the program.
 * @param file a string representing the name of the file in error
 * @param func a string representing the name of the function in error
 * @param line a positive integer represents the line number in error
 * @param err_code an integer of error code
 * @param exit_code an integer of exit code
 */
_Noreturn void fatal_errno(const char *file, const char *func, const size_t line, int err_code, int exit_code)   // NOLINT(bugprone-easily-swappable-parameters)
{
    const char *msg;

    msg = strerror(err_code);                                                                  // NOLINT(concurrency-mt-unsafe)
    fprintf(stderr, "Error (%s @ %s:%zu %d) - %s\n", file, func, line, err_code, msg);  // NOLINT(cert-err33-c)
    exit(exit_code);                                                                            // NOLINT(concurrency-mt-unsafe)
}

/**
 * Print error message and exit the program.
 * @param file a string representing the name of the file in error
 * @param func a string representing the name of the function in error
 * @param line a positive integer represents the line number in error
 * @param msg a string represents a message about the error
 * @param exit_code an integer of exit code
 */
_Noreturn void fatal_message(const char *file, const char *func, const size_t line, const char *msg, int exit_code)
{
    fprintf(stderr, "Error (%s @ %s:%zu) - %s\n", file, func, line, msg);  // NOLINT(cert-err33-c)
    exit(exit_code);                                                               // NOLINT(concurrency-mt-unsafe)
}
