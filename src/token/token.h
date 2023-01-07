#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>

enum token_type
{
    WORD = 0,
    SEMICOLON,
    NEWLINE,
    AND,
    OR,
    PIPE,
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_PARENTHESE,
    CLOSE_PARENTHESE,
    FOR,
    DO,
    DONE,
    WHILE,
    UNTIL,
    CASE,
    ESAC,
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    DOUBLE_SEMICOLON,
    ERROR,
    // TODO redirections token, ENV_VARIABLE ?, ...
};

#define CREATE_DICO(Name) char *Name[] = { \
    [WORD] = "", \
    [SEMICOLON] = ";", \
    [NEWLINE] = "\n", \
    [AND] = "&&", \
    [OR] = "||", \
    [PIPE] = "|", \
    [OPEN_BRACE] = "{", \
    [CLOSE_BRACE] = "}", \
    [OPEN_PARENTHESE] = "(", \
    [CLOSE_PARENTHESE] = ")", \
    [FOR] = "for", \
    [DO] = "do", \
    [DONE] = "done", \
    [WHILE] = "while", \
    [UNTIL] = "until", \
    [CASE] = "case", \
    [ESAC] = "esac", \
    [IF] = "if", \
    [THEN] = "then", \
    [ELIF] = "elif", \
    [ELSE] = "else", \
    [FI] = "fi", \
    [DOUBLE_SEMICOLON] = ";;", \
    [ERROR] = NULL, \
}

#define CREATE_DELIMITATORS(Name) char Name[] = { \
    '|', '&', ';', '<', '>', '(', ')', '$', '`', '\\', '\"', '\'', ' ', '\t', '\n', '\0' \
}

struct token
{
    char *symbol;
    enum token_type type;
};

// create a token with the right attributes and returns it.
// returns null if symbol is NULL (wrong execution).
struct token *new_token(char *symbol, enum token_type type);

// free a token and its attributes.
// if the token is NULL, does nothing
void free_token(void *data);

#endif /* !TOKEN_H */
