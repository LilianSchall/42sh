#include "parser.h"

static struct AST *correct_pipeline_tree(struct AST *neg_tree,
        struct AST *pipe_tree, struct AST *command_tree)
{
    if (neg_tree)
    {
        list_append(neg_tree->linked_list,
                    pipe_tree ? pipe_tree : command_tree);
        // returns an operator AST
        return neg_tree;
    }
    else if (pipe_tree)
    {
        // returns a pipe AST
        return pipe_tree;
    }
    // returns a sequence AST
    return command_tree;
}

struct AST *pipeline_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *token = list_head(token_list);
    struct AST *neg_tree = NULL;
    struct AST *pipe_tree = NULL;
    struct AST *command_tree = NULL;
    char *err = "WORD";
    if (!token)
        goto pipeline_error;

    if (token->type == NEG)
    {
        list_pop(token_list);
        neg_tree = new_AST(token, OPERATOR, new_list());
    }
    // call of command rule gives a sequence AST
    command_tree = command_rule(token_list, trigger_warn);

    if (!command_tree)
    {
        trigger_warn = false; // we don't need to print another error warning
        goto pipeline_error;
    }

    token = list_head(token_list);
    while (token && token->type == R_PIPE)
    {
        list_pop(token_list);

        if (!pipe_tree)
        {
            pipe_tree = new_AST(token, PIPE, new_list());
            list_append(pipe_tree->linked_list, command_tree);
            // dereference command_tree so that there is no crossreference
            command_tree = NULL;
        }
        else
            free_token(token);

        command_tree = command_rule(token_list, trigger_warn);
        if (!command_tree)
        {
            trigger_warn = false;
            goto pipeline_error;
        }
        list_append(pipe_tree->linked_list, command_tree);
        token = list_head(token_list);
    }
    
    return correct_pipeline_tree(neg_tree, pipe_tree, command_tree);

pipeline_error:
    if (trigger_warn)
        warnx("missing word %s at pipeline_rule", err);

    free_AST(command_tree);
    free_AST(pipe_tree);
    free_AST(neg_tree);
    return NULL;
}
