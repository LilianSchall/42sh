#ifndef AST_H
#define AST_H

#include <stddef.h>

#include "garbage_collector/garbage_collector.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

struct env
{
    char **argv;
    struct linked_list *functions;
};

enum AST_type
{
    CASE_C,
    HERE_DOC,
    D_SUBSHELL, // $(..)
    SUBSHELL, // (..)
    FUNCTION,
    REDIRECTION,
    PIPE,
    OPERATOR, // ! && ||
    ITER, // things to iter on a for loop
    ASSIGNMENT, // =
    COMMAND,
    ARG,
    SEQUENCE,
    CONDITION, // if, elif, else clause
    ARITH
};

struct AST
{
    struct token *value;
    enum AST_type type;
    struct linked_list *linked_list;
};

// create a tree with the right attributes and returns it.
// returns NULL if value is NULL (wrong execution).
struct AST *new_AST(struct token *value, enum AST_type type,
                    struct linked_list *linked_list);

// free a every node of an AST tree
// if tree is NULL, does nothing
void free_AST(void *data);

void remove_AST(struct AST *tree, enum AST_type type);

// pretty_print an AST
void pretty_printer(struct AST *tree);

#endif /* !AST_H */
