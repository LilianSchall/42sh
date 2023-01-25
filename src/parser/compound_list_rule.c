#include "parser.h"

struct AST *compound_list_rule(struct linked_list *token_list,
                               bool trigger_warn)
{
    struct log_info info = { .sym = "\\n",
                             .rulename = "compound_list_rule",
                             .trigger_warn = false };
    struct token *newline_token = NULL;
    while ((newline_token = consume_token(token_list, NEWLINE, info)) != NULL)
        free_token(newline_token);

    struct AST *tree = NULL;
    struct AST *command = and_or_rule(token_list, trigger_warn);

    if (!command)
        return NULL;

    if (command->type == SEQUENCE)
        tree = command;
    else
    {
        tree = new_AST(NULL, SEQUENCE, new_list());
        list_append(tree->linked_list, command);
    }

    while (true)
    {
        info.sym = ";";
        struct token *tmp = consume_token(token_list, SEMICOLON, info);

        if (!tmp)
        {
            info.sym = "\\n";
            tmp = consume_token(token_list, NEWLINE, info);

            if (!tmp)
            {
                warnx("Missing NEWLINE or SEMICOLON at end of compound_list "
                      "rule");
                free_AST(tree);
                return NULL;
            }
        }
        free_token(tmp);
        purge_newline_token(token_list);

        command = and_or_rule(token_list, false);

        if (!command)
            break;

        blend_sequence_AST(tree, command);
    }

    purge_newline_token(token_list);

    return tree;
}
