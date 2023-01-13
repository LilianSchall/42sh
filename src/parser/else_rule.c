#include "parser.h"

struct AST *else_clause_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct log_info info = { .sym = "ELSE",
                             .rulename = "else_clause_rule",
                             .trigger_warn = trigger_warn };

    struct token *else_token = consume_token(token_list, ELSE, info);

    if (else_token)
    {
        struct AST *tree = compound_list_rule(token_list, trigger_warn);

        if (!tree)
            return NULL;
        return combine_token_AST(tree, else_token);
    }
    info.sym = "ELIF";
    struct token *elif_token = consume_token(token_list, ELIF, info);

    if (!elif_token)
        return NULL;
    free_token(elif_token);

    struct AST *cond = compound_list_rule(token_list, trigger_warn);

    if (!cond)
        return NULL;

    info.sym = "THEN";
    struct token *then_token = consume_token(token_list, THEN, info);
    if (!then_token)
    {
        free_AST(cond);
        return NULL;
    }

    struct AST *then_clause = compound_list_rule(token_list, trigger_warn);

    if (!then_clause)
    {
        free_AST(cond);
        free_token(then_token);
        return NULL;
    }
    then_clause = combine_token_AST(then_clause, then_token);

    struct AST *else_clause = else_clause_rule(token_list, false);

    struct AST *tree =
        new_AST(new_token(strdup("else"), ELSE, false), SEQUENCE, new_list());
    struct AST *if_tree =
        new_AST(new_token(strdup("if"), IF, false), CONDITION, new_list());

    list_append(tree->linked_list, if_tree);
    list_append(if_tree->linked_list, cond);
    list_append(if_tree->linked_list, then_clause);
    list_append(if_tree->linked_list,
                else_clause); // if it is null, it will do nothing

    return tree;
}
