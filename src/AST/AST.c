#include "AST.h"

struct AST *new_AST(struct token *value, enum AST_type type,
                    struct linked_list *linked_list)
{
    struct AST *my_AST = mem_malloc(sizeof(struct AST));

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
    mem_free(tree);
}

void remove_AST(struct AST *tree, enum AST_type type)
{
    if (!tree)
        return;
    struct linked_list *children = tree->linked_list;
    struct linked_node *head = children->head;

    struct linked_list *tmp_interface = mem_malloc(sizeof(struct linked_list));

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

