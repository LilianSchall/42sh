#include "AST.h"

static void __pretty_printer(struct AST *tree);

static void print_redirection(struct AST *tree)
{
    struct linked_node *node = tree->linked_list->head;
    struct AST *io = node->data;
    node = node->next;

    struct AST *exec = node->data;
    node = node->next;
    struct AST *to = node->data;

    printf("redirect %s to %s: ", io->value->symbol, to->value->symbol);
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
    printf("command %s ", tree->value->symbol);
    if (!tree->linked_list)
        return;
    for (struct linked_node *node = tree->linked_list->head; node;
                node = node->next)
    {
        struct AST *child = node->data;
            printf("%s ", child->value->symbol);
        } 
 
}

static void print_condition(struct AST *tree)
{
printf("condition %s: ", tree->value->symbol);
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
        printf("assign: %s = %s", name->value->symbol,
                value->value->symbol);
}

static void print_operator(struct AST *tree)
{
printf("operator [%s]: ", tree->value->symbol);
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

    if (tree->type == REDIRECTION)
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
        printf("arg: %s", tree->value->symbol);
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
