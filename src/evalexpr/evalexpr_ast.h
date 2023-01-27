#ifndef EVALEXPR_AST_H
#define EVALEXPR_AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum node_type
{
    NUMBER = 0,
    LB,
    RB,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    POW,
};

struct node
{
    enum node_type type;
    int value;
    int unary;
    struct node *left_child;
    struct node *right_child;
};

int ast_evaluate(const struct node *root, int *err);

struct node *ast_build(char *str, int *err);

void ast_print(const struct node *root);

void ast_delete(struct node *root);

#endif /* !EVALEXPR_AST_H */
