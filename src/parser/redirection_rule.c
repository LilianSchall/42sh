#include "parser.h"

static struct token *get_standard_io_number(enum token_type type)
{
    if (type == R_SUP || type == R_SUP_SUP || type == R_SUP_AND
        || type == R_SUP_PIPE)
        return new_token(
            new_unique_symbols(gc_strdup("1"), false, false, false), IO_NUMBER);
    // else
    return new_token(new_unique_symbols(gc_strdup("0"), false, false, false),
                     IO_NUMBER);
}

static struct AST *parse_target(struct linked_list *token_list,
                                bool *is_heredoc)
{
    struct token *token = list_head(token_list);

    if (!token)
        return NULL;

    if (token->type == HEREDOC)
    {
        *is_heredoc = true;
        list_pop(token_list);
        free_token(token);
        token = list_head(token_list);
    }

    if (!token || token->type != WORD)
    {
        return NULL;
    }
    list_pop(token_list);

    return new_AST(token, ARG, NULL);
}

struct AST *redirection_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *token = list_head(token_list);
    struct token *io_number = NULL;

    char *err = "WORD";

    if (!token)
        goto redirection_error;

    if (token->type == IO_NUMBER)
    {
        io_number = token;
        list_pop(token_list);
        token = list_head(token_list);

        if (!token)
        {
            err = "REDIRECT";
            goto redirection_error;
        }
    }

    if (!is_redirect(token))
    {
        err = "REDIRECT";
        goto redirection_error;
    }

    list_pop(token_list);
    bool is_heredoc = false;
    struct AST *target = parse_target(token_list, &is_heredoc);

    struct AST *redirect_tree = new_AST(token, is_heredoc
        ? HERE_DOC : REDIRECTION, new_list());
    if (!word)
    {
        err = "WORD OR HEREDOCS";
        goto redirection_error;
    }
    list_pop(token_list);

    if (!io_number)
        io_number = get_standard_io_number(redirect_tree->value->type);

    list_append(redirect_tree->linked_list, new_AST(io_number, ARG, NULL));
    list_append(redirect_tree->linked_list, new_AST(word, ARG, NULL));

    return redirect_tree;

redirection_error:
    if (trigger_warn)
        warnx("missing word %s at redirection_rule", err);
    return NULL;
}
