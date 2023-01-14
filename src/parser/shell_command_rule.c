#include "parser.h"

struct AST *shell_command_rule(struct linked_list *token_list,
                               bool trigger_warn)
{
    struct token *token = list_head(token_list);

    if (!token)
        goto shell_command_error;
    if (token->type == IF)
        return rule_if_rule(token_list, trigger_warn);
    else if (token->type == WHILE)
        return rule_while_rule(token_list, trigger_warn);
    else if (token->type == UNTIL)
        return rule_until_rule(token_list, trigger_warn);
    else if (token->type == FOR)
        return rule_for_rule(token_list, trigger_warn);

shell_command_error:
    if (trigger_warn)
        warnx("Missing word at shell_command_rule");
    return NULL;
}

