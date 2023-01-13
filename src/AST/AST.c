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

void remove_AST(struct AST *tree, enum AST_type type)
{
    if (!tree)
        return;
    struct linked_list *children = tree->linked_list;
    struct linked_node *head = children->head;

    struct linked_list *tmp_interface = malloc(sizeof(struct linked_list));

    tmp_interface->head = head;
    children->head = NULL;


    struct AST *child = list_head(tmp_interface);
    while (!child)
    {
        list_pop(tmp_interface);
        if (child->type == type)
        {
            free_AST(child);
        }
        else
            list_append(children, child);
        child = list_head(tmp_interface);
    }

    free_list(tmp_interface);
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
    else if (tree->type == PIPE)
    {
        printf("pipe: from ");
        __pretty_printer(tree->linked_list->head->data);
        printf(" to ");
        __pretty_printer(tree->linked_list->head->next->data);
    }
    else if (tree->type == ASSIGNMENT)
    {
        struct AST *name = tree->linked_list->head->data;
        struct AST *value = tree->linked_list->head->next->data;
        printf("assign: %s = %s", name->value->symbol,
                value->value->symbol);
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
