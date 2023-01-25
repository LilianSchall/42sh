#include "parser.h"

static struct AST *command_block_subrule(struct linked_list *token_list,
                                         bool trigger_warn)
{
    struct token *token = list_head(token_list);
    list_pop(token_list);
    free_token(token);

    struct AST *compound = compound_list_rule(token_list, trigger_warn);

    if (!compound)
        return NULL;

    token = list_head(token_list);

    if (!token || token->type != CLOSE_BRACE)
    {
        warnx("Missing CLOSE_BRACE at shell_command_rule");
        free_AST(compound);
        return NULL;
    }

    list_pop(token_list);
    free_token(token);

    return root_sequence_if_needed(compound);
}

static struct AST *subshell_subrule(struct linked_list *token_list,
                                    bool trigger_warn)
{
    struct token *token = list_head(token_list);
    list_pop(token_list);

    free_token(token);

    struct AST *compound = compound_list_rule(token_list, trigger_warn);

    if (!compound)
        return NULL;

    token = list_head(token_list);

    if (token->type != CLOSE_PARENTHESE)
    {
        warnx("Missing CLOSE_PARENTHSE at shell_command_rule");
        free_AST(compound);
        return NULL;
    }
    list_pop(token_list);
    free_token(token);

    struct AST *tree = new_AST(NULL, SUBSHELL, new_list());
    list_append(tree->linked_list, compound);

    return tree;
}

struct AST *shell_command_rule(struct linked_list *token_list,
                               bool trigger_warn)
{
    struct token *token = list_head(token_list);

    if (!token)
        goto shell_command_error;
    if (token->type == OPEN_BRACE)
        return command_block_subrule(token_list, trigger_warn);
    else if (token->type == OPEN_PARENTHESE)
        return subshell_subrule(token_list, trigger_warn);
    else if (token->type == IF)
        return rule_if_rule(token_list, trigger_warn);
    else if (token->type == WHILE)
        return rule_while_rule(token_list, trigger_warn);
    else if (token->type == UNTIL)
        return rule_until_rule(token_list, trigger_warn);
    else if (token->type == FOR)
        return rule_for_rule(token_list, trigger_warn);
    else if (token->type == CASE)
        return rule_case_rule(token_list, trigger_warn);

shell_command_error:
    if (trigger_warn)
        warnx("Missing word at shell_command_rule");
    return NULL;
}
