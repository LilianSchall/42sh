#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>

#include "linked_list/linked_list.h"

enum token_type
{
    WORD = 0,
    SEMICOLON,
    NEWLINE,
    AND,
    OR,
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
    IO_NUMBER, // FD to redirect
    R_SUP, // >
    R_SUP_PIPE, // >|
    R_SUP_SUP, // >>
    R_SUP_AND, // >&
    R_INF, // <
    R_INF_AND, // <&
    R_INF_SUP, // <>
    R_PIPE, // |
    ERROR,
    // TODO redirections token, ENV_VARIABLE ?, ...

};

#define CREATE_DICO(Name)                                                      \
    char *Name[] = {                                                           \
        [WORD] = "",                                                           \
        [SEMICOLON] = ";",                                                     \
        [NEWLINE] = "\n",                                                      \
        [AND] = "&&",                                                          \
        [OR] = "||",                                                           \
        [OPEN_BRACE] = "{",                                                    \
        [CLOSE_BRACE] = "}",                                                   \
        [OPEN_PARENTHESE] = "(",                                               \
        [CLOSE_PARENTHESE] = ")",                                              \
        [FOR] = "for",                                                         \
        [DO] = "do",                                                           \
        [DONE] = "done",                                                       \
        [WHILE] = "while",                                                     \
        [UNTIL] = "until",                                                     \
        [CASE] = "case",                                                       \
        [ESAC] = "esac",                                                       \
        [IF] = "if",                                                           \
        [THEN] = "then",                                                       \
        [ELIF] = "elif",                                                       \
        [ELSE] = "else",                                                       \
        [FI] = "fi",                                                           \
        [DOUBLE_SEMICOLON] = ";;",                                             \
        [IO_NUMBER] = "",                                                      \
        [R_SUP] = ">",                                                         \
        [R_SUP_PIPE] = ">|",                                                   \
        [R_SUP_SUP] = ">>",                                                    \
        [R_SUP_AND] = ">&",                                                    \
        [R_INF] = "<",                                                         \
        [R_INF_AND] = "<&",                                                    \
        [R_INF_SUP] = "<>",                                                    \
        [R_PIPE] = "|",                                                        \
        [ERROR] = NULL,                                                        \
    }

#define CREATE_DELIMITATORS(Name)                                              \
    char Name[] = { '|', '&',  ';',  '<',  '>', '(',  ')',  '$', '`',          \
                    '#', '\\', '\"', '\'', ' ', '\t', '\n', '\0' }


#define CREATE_REDIRECTIONS(Name) \
    char *Name[] = { \
        ">>", ">&", "<&", ">|", "<>" } \

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

void print_token_list(struct linked_list *list);

#endif /* !TOKEN_H */
