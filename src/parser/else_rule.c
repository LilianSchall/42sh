#include "parser.h"

static bool parse_cond_clause(struct linked_list *token_list, struct AST **cond,
                              struct AST **then_clause, struct log_info info)
{
    info.sym = "ELIF";
    struct token *elif_token = consume_token(token_list, ELIF, info);

    if (!elif_token)
        return false;
    free_token(elif_token);

    *cond = compound_list_rule(token_list, info.trigger_warn);

    if (!(*cond))
        return false;

    info.sym = "THEN";
    struct token *then_token = consume_token(token_list, THEN, info);
    if (!then_token)
    {
        free_AST(*cond);
        return false;
    }

    *then_clause = compound_list_rule(token_list, info.trigger_warn);

    if (!(*then_clause))
    {
        free_AST(*cond);
        free_token(then_token);
        return false;
    }
    *then_clause = combine_token_AST(*then_clause, then_token);

    return true;
}

struct AST *else_clause_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct log_info info = { .sym = "ELSE",
                             .rulename = "else_clause_rule",
                             .trigger_warn = trigger_warn };

    struct AST *cond = NULL;
    struct AST *then_clause = NULL;

    struct token *else_token = consume_token(token_list, ELSE, info);
    if (else_token)
    {
        struct AST *tree = compound_list_rule(token_list, trigger_warn);

        if (!tree)
            return NULL;
        return combine_token_AST(tree, else_token);
    }

    if (!parse_cond_clause(token_list, &cond, &then_clause, info))
        return NULL;

    struct AST *else_clause = else_clause_rule(token_list, false);

    struct AST *tree = new_AST(
        new_token(new_unique_symbols(gc_strdup("else"), false, false, false),
                  ELSE),
        SEQUENCE, new_list());
    struct AST *if_tree = new_AST(
        new_token(new_unique_symbols(gc_strdup("if"), false, false, false), IF),
        CONDITION, new_list());

    list_append(tree->linked_list, if_tree);
    list_append(if_tree->linked_list, cond);
    list_append(if_tree->linked_list, then_clause);

    // if it is null, it will do nothing
    list_append(if_tree->linked_list, else_clause);
    return tree;
}
