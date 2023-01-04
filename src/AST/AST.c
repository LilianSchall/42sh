#include "AST.h"

struct AST *new_AST(struct token *value, struct AST *left_child,
        struct AST *right_child)
{
    if (value == NULL)
        return NULL;

    struct AST *my_AST = malloc(sizeof(struct AST));
    
    if (value == my_AST)
        return NULL;

    my_AST->value = value;
    my_AST->left_child = left_child;
    my_AST->right_child = right_child;

    return my_AST;
}

void free_AST(struct AST *tree)
{
    if(tree)
    {
        free_AST(tree->left_child);
        free_AST(tree->right_child);

        free_token(token);
        free(tree);
    }
}
