#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "garbage_collector/garbage_collector.h"
#include "linked_list/linked_list.h"
#include "symbol/symbol.h"

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
    DOLL_OPEN_PARENTHESE,
    DOLL_OPEN_PARENTHESE_PARENTHESE,
    CLOSE_PARENTHESE,
    CLOSE_PARENTHESE_PARENTHESE,
    BACKQUOTE,
    ENDBACKQUOTE,
    FOR,
    IN,
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
    INF_INF,
    INF_INF_MIN,
    R_SUP, // >
    R_SUP_PIPE, // >|
    R_SUP_SUP, // >>
    R_SUP_AND, // >&
    R_INF, // <
    R_INF_AND, // <&
    R_INF_SUP, // <>
    R_PIPE, // |
    NEG, // !
    VARASSIGNMENT, // test=3
    HEREDOC,
    ERROR,
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
        [DOLL_OPEN_PARENTHESE] = "$(",                                         \
        [DOLL_OPEN_PARENTHESE_PARENTHESE] = "$((",                             \
        [CLOSE_PARENTHESE] = ")",                                              \
        [CLOSE_PARENTHESE_PARENTHESE] = "))",                                  \
        [BACKQUOTE] = "`",                                                     \
        [ENDBACKQUOTE] = "`",                                                  \
        [FOR] = "for",                                                         \
        [IN] = "in",                                                           \
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
        [INF_INF] = "<<",                                                      \
        [INF_INF_MIN] = "<<-",                                                 \
        [R_SUP] = ">",                                                         \
        [R_SUP_PIPE] = ">|",                                                   \
        [R_SUP_SUP] = ">>",                                                    \
        [R_SUP_AND] = ">&",                                                    \
        [R_INF] = "<",                                                         \
        [R_INF_AND] = "<&",                                                    \
        [R_INF_SUP] = "<>",                                                    \
        [R_PIPE] = "|",                                                        \
        [NEG] = "!",                                                           \
        [VARASSIGNMENT] = "",                                                  \
        [HEREDOC] = "",                                                        \
        [ERROR] = NULL,                                                        \
    }

#define CREATE_DELIMITATORS(Name)                                              \
    char Name[] = { '|', '&',  ';',  '<',  '>', '(',  ')',  '`',               \
                    '#', '\\', '\"', '\'', ' ', '\t', '\n', '\0' }

#define CREATE_NON_DELIMITATOR(Name)                                           \
    enum token_type Name[] = { WORD, OPEN_BRACE, FOR,   IN,                    \
                               DO,   DONE,       WHILE, UNTIL,                 \
                               CASE, ESAC,       IF,    THEN,                  \
                               ELIF, ELSE,       FI,    VARASSIGNMENT,         \
                               NEG,  CLOSE_BRACE }

#define CREATE_REDIRECTIONS(Name)                                              \
    char *Name[] = { ">>", ">&", "<&", ">|", "<>", "<<", "<<-", NULL }

#define CREATE_REDIRECT_SCOUT(Name)                                            \
    enum token_type Name[] = { R_SUP,     R_SUP_PIPE, R_SUP_SUP,               \
                               R_SUP_AND, R_INF,      R_INF_AND,               \
                               R_INF_SUP, INF_INF,    INF_INF_MIN }

struct token
{
    struct symbol **values; // null terminated array containing symbols
    enum token_type type;
};

// create a token with the right attributes and returns it.
// returns null if values is NULL (wrong execution).
struct token *new_token(struct symbol **values, enum token_type type);

// free a token and its attributes.
// if the token is NULL, does nothing
void free_token(void *data);

void print_token_list(struct linked_list *list);

bool is_redirect(struct token *token);

#endif /* !TOKEN_H */
