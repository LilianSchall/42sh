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
