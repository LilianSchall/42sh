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

    if (is_non_delimitator(token->type) || is_substitution_ruled(token->type))
        return substitution_rule(token_list, trigger_warn);
    else
        return redirection_rule(token_list, trigger_warn);
    return tree;
}

