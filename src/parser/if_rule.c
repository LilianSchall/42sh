#include "parser.h"

struct AST *rule_if_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *if_token = NULL;
    struct token *then_token = NULL;
    struct token *tmp = NULL;
    // parsing if token
    struct log_info info = { .sym = "IF",
                             .rulename = "rule_if_rule",
                             .trigger_warn = true };

    if (!(if_token = consume_token(token_list, IF, info)))
        return NULL;

    struct AST *cond = compound_list_rule(token_list, trigger_warn);

    if (cond == NULL)
        return NULL;

    // parsing then token
    info.sym = "THEN";
    if (!(then_token = consume_token(token_list, THEN, info)))
    {
        free_token(if_token);
        free_AST(cond);
        return NULL;
    }

    struct AST *then_clause = compound_list_rule(token_list, trigger_warn);
    if (!then_clause)
    {
        free_token(if_token);
        free_token(then_token);
        free_AST(cond);
        return NULL;
    }

    then_clause = combine_token_AST(then_clause, then_token);

    struct AST *else_clause = else_clause_rule(token_list, false);

    // parsing fi token
    info.sym = "FI";
    if (!(tmp = consume_token(token_list, FI, info)))
    {
        free_token(if_token);
        free_AST(cond);
        free_AST(then_clause);
        free_AST(else_clause);
        return NULL;
    }
    free_token(tmp);

    struct AST *tree = new_AST(if_token, CONDITION, new_list());
    list_append(tree->linked_list, cond);
    list_append(tree->linked_list, then_clause);
    list_append(tree->linked_list, else_clause);

    return tree;
}

