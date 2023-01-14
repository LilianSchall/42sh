#include "parser.h"

struct AST *and_or_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct AST *pipeline = pipeline_rule(token_list, trigger_warn);
    struct AST *tree = NULL;
    if (!pipeline)
        return NULL;

    struct token *token = list_head(token_list);

    // if we didn't found && or ||
    // we just end the rule and returne the pipeline;
    if (!token || (token->type != AND && token->type != OR))
        return pipeline;

    // else we found an operator
    list_pop(token_list);

    // we purge remaining newlines
    purge_newline_token(token_list);

    struct AST *command = and_or_rule(token_list, trigger_warn);

    if (command == NULL)
    {
        free_token(token);
        free_AST(pipeline);

        return NULL;
    }
    pipeline = root_sequence_if_needed(pipeline);
    command = root_sequence_if_needed(command);

    tree = new_AST(token, OPERATOR, new_list());
    list_append(tree->linked_list, pipeline);
    list_append(tree->linked_list, command);

    return tree;
}

