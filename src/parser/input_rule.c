#include "parser.h"

struct AST *input_rule(struct linked_list *token_list)
{
    struct AST *tree = NULL;

    // EOF rule
    if (token_list == NULL)
        return NULL;

    struct token *token = list_head(token_list);

    if (token == NULL)
        return NULL;

    if (token->type == NEWLINE)
        goto end;

    tree = list_rule(token_list);

    // check whether there is a \n token still

    token = list_head(token_list);

    if (token == NULL)
        return tree;

    if (token->type != NEWLINE)
    {
        free_AST(tree);
        warnx("command not found at input_rule");
        return NULL;
    }

end:

    free_token(token);
    list_pop(token_list);
    return tree;
}
