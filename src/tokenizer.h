#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <ctype.h>
#include <string.h>

typedef enum
{
    TOKEN_WORD,
    TOKEN_REDIRECT_OUT, // >
    TOKEN_REDIRECT_IN,  // <
    TOKEN_APPEND,       // >>
    TOKEN_HEREDOC,      // <<
    TOKEN_PIPE,         // |
    TOKEN_AND,          // &&
    TOKEN_OR,           // ||
    TOKEN_SEMICOLON,    // ;
    TOKEN_FD_IN,        // 0>
    TOKEN_FD_APPEND,    // 0>>
    TOKEN_FD_OUT        // 0<
} token_type_t;

typedef struct
{
    token_type_t type;
    char *text;
} token_t;

typedef enum
{
    SKIPPING_WHITESPACES,
    COLLECTING_WORD,
    COLLECTING_OPERATOR
} tokenizer_state_t;

bool isoperator(char *str)
{
    char single_char_operators[] = {'>', '<', '|', '&', ';'};
    for (int i = 0; i < sizeof(single_char_operators); i++)
    {
        if (str[i] == single_char_operators[i])
        {
            return true;
        }
    }
    return false;
}

int tokenize(char *input, token_t *token_array[])
{
    for (int i = 0; i < strlen(input); i++)
    {
    }
    return 0;
}

#endif