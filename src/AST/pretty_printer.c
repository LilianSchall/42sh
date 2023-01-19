#include "AST.h"

static void __pretty_printer(struct AST *tree);

static void print_subshell(struct AST *tree, bool is_redirected)
{
    return;
}

static void print_function(struct AST *tree)
{
    struct AST *name_AST = tree->linked_list->head->data;
    char *name = get_cat_symbols(name_AST->value->values);

    printf("function [%s]:", name);
    tree = tree->linked_list->head->next->data;

    __pretty_printer(tree);

    mem_free(name);
}

static void print_redirection(struct AST *tree)
{
    struct linked_node *node = tree->linked_list->head;
    struct AST *io = node->data;
    node = node->next;

    struct AST *exec = node->data;
    node = node->next;
    struct AST *to = node->data;

    printf("redirect %s to %s: ", io->value->values[0]->value, to->value->values[0]->value);
    __pretty_printer(exec);
}

static void print_sequence(struct AST *tree)
{
    printf("sequence ");
    if (!tree->linked_list)
        return;
    for (struct linked_node *node = tree->linked_list->head; node;
         node = node->next)
    {
        __pretty_printer(node->data);
    }
}

static void print_command(struct AST *tree)
{
    printf("command ");
    if (!tree->linked_list)
        return;
    for (struct linked_node *node = tree->linked_list->head; node;
         node = node->next)
    {
        struct AST *child = node->data;
        printf("%s ", child->value->values[0]->value);
    }
}

static void print_condition(struct AST *tree)
{
    printf("condition %s: ", tree->value->values[0]->value);
    if (!tree->linked_list)
        return;
    for (struct linked_node *node = tree->linked_list->head; node;
         node = node->next)
    {
        __pretty_printer(node->data);
    }
}

static void print_iter(struct AST *tree)
{
    printf("iter: ");
    if (!tree->linked_list)
        return;
    for (struct linked_node *node = tree->linked_list->head; node;
         node = node->next)
    {
        __pretty_printer(node->data);
    }
}

static void print_pipe(struct AST *tree)
{
    printf("pipe: from ");
    __pretty_printer(tree->linked_list->head->data);
    printf(" to ");
    __pretty_printer(tree->linked_list->head->next->data);
}

static void print_assignment(struct AST *tree)
{
    struct AST *name = tree->linked_list->head->data;
    struct AST *value = tree->linked_list->head->next->data;
    printf("assign: %s = %s", name->value->values[0]->value, value->value->values[0]->value);
}

static void print_operator(struct AST *tree)
{
    printf("operator [%s]: ", tree->value->values[0]->value);
    if (!tree->linked_list)
        return;
    for (struct linked_node *node = tree->linked_list->head; node;
         node = node->next)
    {
        __pretty_printer(node->data);
    }
}

static void __pretty_printer(struct AST *tree)
{
    if (tree == NULL)
        return;

    printf("{");
    if (tree->type == D_SUBSHELL || tree->type == SUBSHELL)
        print_subshell(tree, tree->type == D_SUBSHELL);
    else if (tree->type == FUNCTION)
        print_function(tree);
    else if (tree->type == REDIRECTION)
        print_redirection(tree);
    else if (tree->type == SEQUENCE)
        print_sequence(tree);
    else if (tree->type == COMMAND)
        print_command(tree);
    else if (tree->type == CONDITION)
        print_condition(tree);
    else if (tree->type == ITER)
        print_iter(tree);
    else if (tree->type == ARG)
    {
        printf("arg: %s", tree->value->values[0]->value);
    }
    else if (tree->type == PIPE)
        print_pipe(tree);
    else if (tree->type == ASSIGNMENT)
        print_assignment(tree);
    else if (tree->type == OPERATOR)
        print_operator(tree);

    printf("}");
}

void pretty_printer(struct AST *tree)
{
    if (!tree)
    {
        puts("Tree is null.");
        return;
    }

    __pretty_printer(tree);
    printf("\n");
}

