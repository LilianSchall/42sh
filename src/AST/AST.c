#include "AST.h"

struct AST *new_AST(struct token *value, enum AST_type type,
                    struct linked_list *linked_list)
{
    struct AST *my_AST = malloc(sizeof(struct AST));

    if (NULL == my_AST)
        return NULL;

    my_AST->value = value;
    my_AST->type = type;
    my_AST->linked_list = linked_list;

    return my_AST;
}

void free_AST(void *data)
{
    struct AST *tree = data;
    if (tree == NULL)
        return;

    if (tree->linked_list)
        deep_free_list(tree->linked_list, free_AST);

    if (tree->value)
        free_token(tree->value);
    free(tree);
}

static void __pretty_printer(struct AST *tree)
{
    if (tree == NULL)
        return;

    printf("{");

    if (tree->type == REDIRECTION)
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
    else if (tree->type == SEQUENCE)
    {
        printf("sequence ");
        if (!tree->linked_list)
            goto print_end;
        for (struct linked_node *node = tree->linked_list->head; node;
                node = node->next)
        {
            __pretty_printer(node->data);
        }

    }
    else if (tree->type == COMMAND)
    {
        printf("command %s ", tree->value->symbol);
        if (!tree->linked_list)
            goto print_end;
        for (struct linked_node *node = tree->linked_list->head; node;
                node = node->next)
        {
            struct AST *child = node->data;
            printf("%s ", child->value->symbol);
        } 
    }
    else if (tree->type == CONDITION)
    {
        printf("condition %s: ", tree->value->symbol);
        if (!tree->linked_list)
            goto print_end;
        for (struct linked_node *node = tree->linked_list->head; node;
                node = node->next)
        {
            __pretty_printer(node->data);
        }       
    }
    else if (tree->type == ITER)
    {
        printf("iter: ");
        if (!tree->linked_list)
            goto print_end;
        for (struct linked_node *node = tree->linked_list->head; node;
                node = node->next)
        {
            __pretty_printer(node->data);
        }
    }
    else if (tree->type == ARG)
    {
        printf("arg: %s", tree->value->symbol);
    }
print_end:
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
