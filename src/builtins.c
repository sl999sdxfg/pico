#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "builtins.h"
#include "debug.h"

int exit_(char *argv[])
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
            return 1;
        }
        else
        {
            exit(code);
        }
    }
    return 0;
}

int cd(char *argv[], char *cwd, size_t cwd_size)
{
    if (argv[1] == NULL)
    {
        char *home_path = getenv("HOME");
        if (!home_path)
        {
            // may set home_path = '/';
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
        if (chdir(home_path) == -1)
        {
            perror("cd");
        }
        DEBUG_PRINT("home path %s", home_path);
    }
    else if (argv[2] != NULL)
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 2;
    }
    else if (chdir(argv[1]) == -1)
    {
        fprintf(stderr, "cd: wrong path\n");
        // perror("cd");
        return 2;
    }
    // do i have to handle getcwd() == NULL?
    if (getcwd(cwd, cwd_size) == NULL)
    {
        perror("getcwd");
        return 1;
    }
    return 0;
}

int pwd()
{
    char cwd[1024] = {'\0'};
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
        return 0;
    }
    else
    {
        perror("pwd");
        return 1;
    }
}

extern char **environ;
int export(char *argv[])
{
    if (argv[1] != NULL)
    {
        char *variable = strtok(argv[1], "=");
        char *value = strtok(NULL, "=");
        if (variable != NULL && value != NULL)
        {
            DEBUG_PRINT("set %s=%s", variable, value);
            setenv(variable, value, 1);
            DEBUG_PRINT("env value %s=%s", variable, getenv(variable));
        }
        else
        {
            printf("Cant parse variable and value\n");
            return 2;
        }
    }
    else
    {
        // for (char **env = environ; env != NULL; env++)
        for (char **env = environ; *env != NULL; env++)
        {
            // printf("%p\n", env);
            printf("%s\n", *env);
        }
    }
    return 0;
}

int echo(char *argv[])
{
    for (int i = 1; argv[i] != NULL; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}