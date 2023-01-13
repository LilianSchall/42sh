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
        
        // in order for our parser to work we need to carefully purge \n token
        purge_newline_token(token_list);
        // check for and_or rule again

        child = and_or_rule(token_list, false);

        if (child == NULL)
            break;

        list_append(tree->linked_list, child);
        token = list_head(token_list);
    }

    return tree;
}
