#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ansi.h"
#include "builtins.h"
#include "debug.h"
// TODO: refactor macros
// TODO: change perror into fprintf(stderror)
// strerror(errno) where appropriate
// when shell should terminate?
// REFACTOR: no magic constants
// maybe define some structs to clean up mess

#define SHELL_PROMPT(usr, host, cwd) printf(FG_GREEN "%s@%s" COLOR_RESET ":" FG_BLUE \
                                                     "%s" COLOR_RESET "$ ",          \
                                            host, usr, cwd);

int main()
{
    char *input = NULL;
    size_t len = 0;
    ssize_t chars_read;

    // structures to make prompt nice
    // TODO: when SHELL_PROMT first runs should set cwd
    char cwd[1024] = {'/', '\0'};
    char usr[256] = {'\0'};
    char host[64] = {'\0'};

    // is this safe? wouldn't work concurrently. getting username, current dir and hostname
    int uid = getuid();
    struct passwd *pwuser = getpwuid(uid);
    strcpy(usr, pwuser->pw_name);
    gethostname(host, sizeof(host));

    do
    {
        SHELL_PROMPT(host, usr, cwd)

        chars_read = getline(&input, &len, stdin);
        if (chars_read >= 1)
        {
            DEBUG_PRINT("Characters read: %zd", chars_read);
            DEBUG_PRINT("You entered: %s", input);
            DEBUG_PRINT("Buffer length: %lu", len);
        }
        else
        {
            perror("Error reading line");
            printf("\n");
            exit(EXIT_SUCCESS);
        }

        // trim newline at the end of input
        int input_length = strlen(input);
        if (input_length >= 1 && input[input_length] == '\0')
        {
            input[input_length - 1] = '\0';
        }

        // split input by spaces and pass to execvp as argv
        char *argv[128] = {NULL};
        char *token;

        token = strtok(input, " ");
        for (int i = 0; token != NULL; i++)
        {
            argv[i] = token;
            token = strtok(NULL, " ");
        }

        DEBUG_PRINT("Parsed input as argv:");
#ifdef DEBUG
        for (int i = 0; argv[i] != NULL; i++)
        {
            printf("%s\n", argv[i]);
        }
#endif
        // handle whitespaces and newlines
        if (argv[0] == NULL)
        {
            continue;
        }

        // check for builtins
        // const char *builtins[] = {"cd", "echo", "exit", "export", "pwd", "env", "set", "unset", NULL};
        // DEBUG_PRINT("Detected builtin %s", builtins[i]);
        if (strcmp(argv[0], "exit") == 0)
        {
            exit_(argv);
            continue;
        }
        else if (strcmp(argv[0], "cd") == 0)
        {
            cd(argv, cwd, sizeof(cwd));
            continue;
        }
        else if (strcmp(argv[0], "pwd") == 0)
        {
            pwd();
            continue;
        }
        else if (strcmp(argv[0], "export") == 0)
        {
            export(argv);
            continue;
        }
        else if (strcmp(argv[0], "echo") == 0)
        {
            echo(argv);
            continue;
        }

        DEBUG_PRINT("Forking");
        pid_t child = fork();

        if (child == -1)
        {
            perror("Error fork");
        }
        else if (child == 0)
        {
            CHILD_PRINT("Executing user command: %s", input);
            int exec_status = execvp(argv[0], argv);
            if (exec_status == -1)
            {
                perror("Error executing command");
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
                perror("Child exited with error");
            }
            else if (WIFEXITED(status))
            {
                PARENT_PRINT("Child exited normally");
            }
        }
    } while (chars_read != -1);

    free(input);
    return 0;
}