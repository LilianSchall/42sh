#include "parser.h"

static struct token *get_standard_io_number(enum token_type type)
{
    if (type == R_SUP || type == R_SUP_SUP || type == R_SUP_AND ||
            type == R_SUP_PIPE)
        return new_token(strdup("1"), IO_NUMBER);
    // else
    return new_token(strdup("0"), IO_NUMBER);
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
    struct AST *redirect_tree = new_AST(token, REDIRECTION, new_list());

    struct token *word = list_head(token_list);

    if (!word || word->type != WORD)
    {
        err = "WORD";
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
