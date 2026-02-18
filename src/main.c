#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
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

    do
    {
        SHELL_PROMPT
        chars_read = getline(&input, &len, stdin);
        if (chars_read >= 1)
        {
            printf("Characters read: %zd\n", chars_read);
            printf("You entered: %s", input);
            printf("Buffer length %lu\n", len);
        }
        else
        {
            perror("Error reading line");
        }

    } while (chars_read != -1);

    printf("Forking\n");
    int child = fork();
    if (child == -1)
    {
        perror("Error fork");
    }
    else if (child == 0)
    {

        PARENT_PRINT("Running %d", getpid());
        char *const argv[] = {"uname", "-a", NULL};
        PARENT_PRINT("Executing uname -a");
        int exec_status = execvp("uname", argv);
        if (exec_status == -1)
        {
            perror("[PARENT] Error executing command uname -a, terminating");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        CHILD_PRINT("Running %d", getpid());
        char *const argv[] = {"ls", "-la", NULL};
        CHILD_PRINT("Executing ls -la");
        int exec_status = execvp("ls", argv);
        if (exec_status == -1)
        {
            perror("[CHILD] Error executing command ls -la, terminating");
            exit(EXIT_FAILURE);
        }
    }

    free(input);
    return 0;
}