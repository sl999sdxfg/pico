#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "ansi.h"

int main()
{
    char *input = NULL;
    size_t len = 0;
    ssize_t chars_read;

    do
    {
        printf(GREEN "user@host" COLOR_RESET ":" BLUE "~/" COLOR_RESET "$ ");
        chars_read = getline(&input, &len, stdin);
        if (chars_read >= 1)
        {
            printf("Characters read: %zd\n", chars_read);
            printf("You entered: %s", input);
            printf("Buffer length %lu", len);
        }
        printf("\n");
    } while (chars_read != -1);

    if (chars_read == -1)
    {
        perror("Error reading line:");
    }
    printf("\n");
    free(input);
    return 0;
}