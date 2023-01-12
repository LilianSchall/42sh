#include "parser.h"

struct AST *command_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *token = list_head(token_list);

    if (token == NULL)
    {
        if (trigger_warn)
            warnx("Missing token at command_rule");
        return NULL;
    }

    if (token->type == WORD || token->type == IO_NUMBER || is_redirect(token))
        return simple_command_rule(token_list, trigger_warn);
    else if (token->type == IF || token->type == WHILE || token->type == UNTIL)
    {
        struct AST *shell_com_tree = shell_command_rule(token_list, trigger_warn);
        return shell_com_tree;
    }
    // else
    if (trigger_warn)
        warnx("No match for token: %s at command_rule", token->symbol);
    return NULL;
}
