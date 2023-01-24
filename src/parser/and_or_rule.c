#include "parser.h"

static struct AST *and_or_subrule(struct linked_list *token_list,
                                  bool trigger_warn, struct AST *operator)
{
    struct AST *pipeline = pipeline_rule(token_list, trigger_warn);
    struct AST *tree = NULL;
    if (!pipeline)
        return NULL;

    struct token *token = list_head(token_list);

    // if we didn't found && or ||
    // we just end the rule and returne the pipeline;
    if (!token || (token->type != AND && token->type != OR))
    {
        if (!operator)
            return pipeline;
        pipeline = root_sequence_if_needed(pipeline);
        list_append(operator->linked_list, pipeline);

        return operator;
    }

    // else we found an operator
    list_pop(token_list);

    // we purge remaining newlines
    purge_newline_token(token_list);

    tree = new_AST(token, OPERATOR, new_list());
    pipeline = root_sequence_if_needed(pipeline);

    if (operator)
    {
        list_append(operator->linked_list, pipeline);
        operator= root_sequence_if_needed(operator);
        list_append(tree->linked_list, operator);
    }
    else
        list_append(tree->linked_list, pipeline);

    struct AST *command = and_or_subrule(token_list, trigger_warn, tree);

    if (command == NULL)
    {
        free_AST(tree);
        return NULL;
    }

    return command;
}

struct AST *and_or_rule(struct linked_list *token_list, bool trigger_warn)
{
    return and_or_subrule(token_list, trigger_warn, NULL);
}
