#include "parser.h"

static struct AST *handle_redirection(struct linked_list *token_list,
                                      struct AST *tree, bool trigger_warn)
{
    if (tree == NULL)
        return NULL;

    struct AST *redirect_tree = NULL;
    struct AST *last_redirect_tree = NULL;

    struct token *token = list_head(token_list);

    while (token != NULL && (token->type == IO_NUMBER || is_redirect(token)))
    {
        struct AST *child = redirection_rule(token_list, trigger_warn);

        if (child == NULL)
        {
            free_AST(redirect_tree);
            free_AST(tree);
            return NULL;
        }

        if (!redirect_tree)
            redirect_tree = child;
        if (last_redirect_tree != NULL)
            list_insert(last_redirect_tree->linked_list, child, 1);
        last_redirect_tree = child;
        token = list_head(token_list);
    }

    if (redirect_tree)
    {
        list_insert(last_redirect_tree->linked_list, tree, 1);
        return redirect_tree;
    }
    return tree;
}

struct AST *command_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *token = list_head(token_list);
    struct AST *tree = new_AST(NULL, SEQUENCE, new_list());
    if (token == NULL)
    {
        if (trigger_warn)
            warnx("Missing token at command_rule");
        free_AST(tree);
        return NULL;
    }

    if (token->type == WORD || token->type == IO_NUMBER || is_redirect(token)
        || token->type == VARASSIGNMENT)
    {
        struct AST *command_tree =
            simple_command_rule(token_list, trigger_warn);

        if (!command_tree)
        {
            free_AST(tree);
            return NULL;
        }
        list_append(tree->linked_list, command_tree);
    }
    else if (token->type == IF || token->type == WHILE || token->type == UNTIL
             || token->type == FOR || token->type == OPEN_BRACE)
    {
        struct AST *shell_com_tree =
            shell_command_rule(token_list, trigger_warn);

        if (!shell_com_tree)
        {
            free_AST(tree);
            return NULL;
        }

        // purge newline token
        purge_newline_token(token_list);

        list_append(
            tree->linked_list,
            handle_redirection(token_list, shell_com_tree, trigger_warn));
    }
    else
    {
        if (trigger_warn)
            warnx("No match for token at command_rule");
        free_AST(tree);
        return NULL;
    }

    return tree;
}

