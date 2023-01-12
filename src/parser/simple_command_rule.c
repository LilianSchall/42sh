#include "parser.h"

static void stick_correctly_child(struct AST **tree,
        struct AST **redirect_tree, struct AST *child)
{
    
}

struct AST *simple_command_rule(struct linked_list *token_list,
                                bool trigger_warn)
{
    struct AST *tree = NULL;
    struct token *token = list_head(token_list);
    
    struct redirect_tree = NULL;

    if (token == NULL)
    {
        if (trigger_warn)
            warnx("Missing word WORD at simple_command_rule");
        return tree;
    }

    if (token->type != WORD)
    {
        if (trigger_warn)
            warnx("%s: simple_command missmatch", token->symbol);
        goto simple_command_end;
    }

    tree = new_AST(token, COMMAND, NULL);
    list_pop(token_list);

    token = list_head(token_list);
    while (token != NULL && (token->type == WORD ||
           token->type == IO_NUMBER || is_redirect(token)))
    {
        struct AST *child = element_rule(token_list, trigger_warn);

        if (child == NULL)
        {
            free_AST(tree);
            free_AST(redirect_tree);
            tree = NULL;
            goto simple_command_end;
        }

        if (!tree->linked_list)
            tree->linked_list = new_list();

        stick_correctly_new_child(&tree, &redirect_tree, child);

        // make a look ahead of 1 to know when to stop
        token = list_head(token_list);
    }

simple_command_end:
    return tree;
}
