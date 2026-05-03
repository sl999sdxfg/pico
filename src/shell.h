#ifndef SHELL_H
#define SHELL_H

#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ansi.h"

#define MAX_ARGS 128

#define SHELL_PROMPT printf(FG_GREEN "user@host" COLOR_RESET ":" FG_BLUE "~/" COLOR_RESET "$ ");

#ifdef DEBUG
#define DEBUG_PRINT(format, ...)                       \
    fprintf(stdout, "DEBUG: %s:%d:%s(): " format "\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define PARENT_PRINT(fmt, ...) printf(FG_BRIGHT_GREEN "[PARENT]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#define CHILD_PRINT(fmt, ...) printf(FG_BRIGHT_CYAN "[CHILD]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#else
#define DEBUG_PRINT(...) ((void)0)
#define CHILD_PRINT(...) ((void)0)
#define PARENT_PRINT(...) ((void)0)
#endif

/**
 * Trim the trailing newline from the input string, in place.
 */
void trim_newline(char *str);

/**
 * Split the input string by whitespace into argv[].
 * Returns the number of tokens (argc).
 * argv must have room for at least MAX_ARGS entries, and will be NULL-terminated.
 */
int tokenize(char *input, char *argv[]);

/**
 * Try to execute a builtin command.
 * Returns true if argv[0] was a builtin (and was handled), false otherwise.
 */
bool execute_builtin(int argc, char *argv[]);

/**
 * Fork and exec an external command, waiting for it to finish.
 */
void execute_external(char *argv[], const char *input);

#endif
