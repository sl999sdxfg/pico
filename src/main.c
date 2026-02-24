#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ansi.h"

#define SHELL_PROMPT printf(FG_GREEN "user@host" COLOR_RESET ":" FG_BLUE "~/" COLOR_RESET "$ ");
#define CHILD_PRINT(fmt, ...) printf(FG_BRIGHT_CYAN "[CHILD]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#define PARENT_PRINT(fmt, ...) printf(FG_BRIGHT_GREEN "[PARENT]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)

int main()
{
    char *input = NULL;
    size_t len = 0;
    ssize_t chars_read;

    // FIX: heap buffer overflow on input ctrl+D

    SHELL_PROMPT
    chars_read = getline(&input, &len, stdin);
    if (chars_read >= 1)
    {
        printf("Characters read: %zd\n", chars_read);
        printf("You entered: %s", input);
        printf("Buffer length: %lu\n", len);
    }
    else
    {
        perror("Error reading line");
    }

    // trim newline at the end of input
    int input_length = strlen(input);
    if (input_length >= 1 && input[input_length] == '\0')
    {
        input[input_length - 1] = '\0';
    }

    // parse input as array of arguments to work with execv
    char *argv[128] = {NULL};
    char *token;
    if (strstr(input, " ") != NULL)
    {
        token = strtok(input, " ");
        argv[0] = token;
        for (int i = 1; token != NULL; i++)
        {
            token = strtok(NULL, " ");
            argv[i] = token;
        }
    }
    else
    {
        argv[0] = input;
    }

    printf("Parsed input:\n");
    for (int i = 0; argv[i] != NULL; i++)
    {
        printf("%s\n", argv[i]);
    }

    printf("Forking\n");
    pid_t child = fork();

    if (child == -1)
    {
        perror("Error fork");
    }
    else if (child == 0)
    {
        CHILD_PRINT("Executing ls");
        int exec_status = execvp("ls", (char *[]){"-la", NULL});
        if (exec_status == -1)
        {
            perror("[CHILD] Error executing command ls -la, terminating");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int status;
        PARENT_PRINT("Running %d", getpid());
        PARENT_PRINT("Waiting on child %d", child);
        waitpid(child, &status, 0);
        if (WIFSIGNALED(status))
        {
            perror("Error\n");
        }
        else if (WIFEXITED(status))
        {
            PARENT_PRINT("Child exited normally");
        }

        PARENT_PRINT("Executing %s", input);
        int exec_status = execvp(argv[0], argv);
        if (exec_status == -1)
        {
            perror("[PARENT] Error executing command");
        }
    }

    free(input);
    return 0;
}