#include "AST.h"

struct AST *new_AST(struct token *value, enum AST_type type, 
        struct linked_list *linked_list)
{
    if (value == NULL || type == NULL)
        return NULL;

    struct AST *my_AST = malloc(sizeof(struct AST));
    
    if (value == my_AST)
        return NULL;

    my_AST->value = value;
    my_AST->type = type;
    my_AST->linked_list = linked_list;

    return my_AST;
}

void free_AST(struct AST *tree)
{
    if(tree)
    {
        // free all childs
        list_deep_free(my_AST->linked_list, free_AST());

        free_token(token);
        free(tree);
    }
}
