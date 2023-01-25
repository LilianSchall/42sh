#include "parser.h"

struct AST *substitution_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *token = list_head(token_list);

    if (!token
        || (!is_substitution_ruled(token->type)
            && !is_non_delimitator(token->type)))
        return NULL;

    list_pop(token_list);

    // we return an ARG AST
    if (is_non_delimitator(token->type))
        return new_AST(token, ARG, NULL);

    // it was '$(' so we free it
    free_token(token);

    token = list_head(token_list);

    struct AST *compound = 
        token->type != CLOSE_PARENTHESE && token->type != ENDBACKQUOTE 
        ? list_rule(token_list)
        : new_AST(NULL, SEQUENCE, new_list());

    if (!compound)
        return NULL;

    token = list_head(token_list);

    if (!token || (token->type != CLOSE_PARENTHESE 
        && token->type != ENDBACKQUOTE))
    {
        warnx("Missing CLOSE_PARENTHESE or ENDBACKQUOTE at substitution_rule");
        free_AST(compound);
        return NULL;
    }

    list_pop(token_list);
    free_token(token);

    struct AST *tree = new_AST(NULL, D_SUBSHELL, new_list());
    list_append(tree->linked_list, compound);

    return tree;
}
