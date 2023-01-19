#include "parser.h"

struct AST *funcdec_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *name = list_head(token_list);

    if (!name || name->type != WORD)
        goto funcdec_error;

    list_pop(token_list);

    struct token *token = NULL;
    struct log_info infos = {
        .sym = "(", .rulename = "funcdec_rule", .trigger_warn = trigger_warn };

    if (!(token = consume_token(token_list, OPEN_PARENTHESE, infos)))
    {
        free_token(name);
        return NULL;
    }
    free_token(token);
    infos.sym = ")";
    if (!(token = consume_token(token_list, OPEN_PARENTHESE, infos)))
    {
        free_token(name);
        return NULL;
    }

    purge_newline_token(token_list);

    struct AST *shell_command = shell_command_rule(token_list, trigger_warn);

    if (!shell_command)
    {
        free_token(name);
        return NULL;
    }

    struct AST *tree = new_AST(NULL, FUNCTION, new_list());

    list_append(tree->linked_list, new_AST(name, ARG, NULL));
    list_append(tree->linked_list, shell_command);

    return tree;


funcdec_error:
    if (trigger_warn)
        warnx("Missing word at funcdec_rule");
    return NULL;
}
