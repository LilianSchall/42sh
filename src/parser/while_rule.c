#include "parser.h"

struct AST *rule_while_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *while_token = NULL;
    struct token *do_token = NULL;
    struct token *tmp = NULL;

    struct log_info info = { .sym = "WHILE",
                             .rulename = "rule_while_rule",
                             .trigger_warn = true };

    // parsing while token
    if (!(while_token = consume_token(token_list, WHILE, info)))
        return NULL;

    struct AST *cond = compound_list_rule(token_list, trigger_warn);

    if (cond == NULL)
        return NULL;

    // parsing do token
    info.sym = "DO";
    if (!(do_token = consume_token(token_list, DO, info)))
    {
        free_token(while_token);
        free_AST(cond);
        return NULL;
    }

    struct AST *do_clause = compound_list_rule(token_list, trigger_warn);

    if (!do_clause)
    {
        free_token(while_token);
        free_token(do_token);
        free_AST(cond);
        return NULL;
    }

    do_clause = combine_token_AST(do_clause, do_token);

    // parsing done token
    info.sym = "DONE";
    if (!(tmp = consume_token(token_list, DONE, info)))
    {
        free_token(while_token);
        free_AST(cond);
        free_AST(do_clause);
        return NULL;
    }

    free_token(tmp);
    struct AST *tree = new_AST(while_token, CONDITION, new_list());
    list_append(tree->linked_list, cond);
    list_append(tree->linked_list, do_clause);

    return tree;
}
