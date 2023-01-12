#include "parser.h"

struct AST *element_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *token = list_head(token_list);
    struct AST *tree = NULL;

    if (token == NULL)
    {
        if (trigger_warn)
            warnx("Missing word WORD at element_rule");
        return tree;
    }

    if (token->type == WORD)
    {
        tree = new_AST(token, ARG, NULL);
        list_pop(token_list);
        return tree;
    }
    else
        return redirection_rule(token_list, trigger_warn);
    return tree;
}
