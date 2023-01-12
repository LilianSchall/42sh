#include "parser.h"

struct AST *list_rule(struct linked_list *token_list)
{
    struct AST *child = and_or_rule(token_list, true);

    if (child == NULL)
        return NULL;

    struct AST *tree = new_AST(NULL, SEQUENCE, new_list());

    list_append(tree->linked_list, child);

    struct token *token = list_head(token_list);
    while (token != NULL && token->type == SEMICOLON)
    {
        free_token(token);
        list_pop(token_list);
        
        // swipe up any \n token clean

        token = list_head(token_list);

        while (token != NULL && token->type == NEWLINE)
        {
            list_pop(token_list);
            free_token(token);
            token = list_head(token_list);
        }

        // check for and_or rule again

        child = and_or_rule(token_list, false);

        if (child == NULL)
            break;

        list_append(tree->linked_list, child);
        token = list_head(token_list);
    }

    return tree;
}
