#include "parsing_tools.h"

struct token *consume_token(struct linked_list *token_list,
                            enum token_type type, struct log_info info)
{
    struct token *token = list_head(token_list);

    if (!token)
    {
        if (info.trigger_warn)
            warnx("Missing word %s at %s", info.sym, info.rulename);
        return NULL;
    }

    if (token->type != type)
    {
        if (info.trigger_warn)
            warnx("%s: %s missmatch at %s", token->symbol, info.sym,
                  info.rulename);
        return NULL;
    }

    list_pop(token_list);

    return token;
}

struct AST *combine_token_AST(struct AST *tree, struct token *token)
{
    if (tree == NULL)
        return NULL;

    tree->value = token;
    return tree;
}
