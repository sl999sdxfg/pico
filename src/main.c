#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ansi.h"

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
            DEBUG_PRINT("Characters read: %zd", chars_read);
            DEBUG_PRINT("You entered: %s", input);
            DEBUG_PRINT("Buffer length: %lu", len);
        }
        else
        {
            // perror("Error reading line");
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
        const char *builtins[] = {"cd", "echo", "exit", "export", "pwd", "env", "set", "unset", NULL};
        bool builtin_detected = false;
        for (int i = 0; builtins[i] != NULL; i++)
        {
            if (strcmp(argv[0], builtins[i]) == 0)
            {
                builtin_detected = true;
                DEBUG_PRINT("Detected builtin %s", builtins[i]);
                break;
            }
        }

        if (builtin_detected)
        {
            if (strcmp(argv[0], "exit") == 0)
            {
                if (argv[1] == NULL || (strcmp(argv[1], "0") == 0))
                {
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    // TODO: replace atoi to something with better error handling
                    int code = atoi(argv[1]);
                    if (code == 0)
                    {
                        printf("exit: Invalid argument\n");
                    }
                    else
                    {
                        exit(code);
                    }
                }
            }
            else if (strcmp(argv[0], "cd") == 0)
            {
                if (argv[1] == NULL)
                {
                    char *home_path = getenv("HOME");
                    if (chdir(home_path) == -1)
                    {
                        perror("cd");
                    }
                    DEBUG_PRINT("home path %s", home_path);
                }
                else
                {
                    if (chdir(argv[1]) == -1)
                    {
                        perror("cd");
                    }
                }
            }
            else if (strcmp(argv[0], "pwd") == 0)
            {
                char cwd[1024] = {'\0'};
                if (getcwd(cwd, sizeof(cwd)) != NULL)
                {
                    printf("%s\n", cwd);
                }
                else
                {
                    perror("pwd");
                }
            }

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