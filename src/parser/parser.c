#include "parser.h"

static struct AST *input_rule(struct linked_list *token_list);
static struct AST *list_rule(struct linked_list *token_list);
struct AST *and_or_rule(struct linked_list *token_list);
struct AST *pipeline_rule(struct linked_list *token_list);
struct AST *command_rule(struct linked_list *token_list);
struct AST *simple_command_rule(struct linked_list *token_list);
struct AST *element_rule(struct linked_list *token_list);

struct AST *input_rule(struct linked_list *token_list)
{
    struct AST *tree = NULL;

    // EOF rule
    if (token_list == NULL)
        return NULL;

    struct token *token = list_head(token_list);
    
    if (token == NULL)
        return NULL;

    if (token->type == NEWLINE)
        goto end;

    tree = list_rule(token_list);
    
    // check whether there is a \n token still

    token = list_head(token_list);

    if (token == NULL)
        return tree;

    if (token->type != NEWLINE)
    {
        warnx("%s: command not found", token->symbol);
        return NULL;
    }
    
    end:

    free_token(token);
    list_pop(token_list);
    return tree;
}

struct AST *list_rule(struct linked_list *token_list)
{
    return and_or_rule(token_list);
}

struct AST *and_or_rule(struct linked_list *token_list)
{
    return pipeline_rule(token_list);
}

struct AST *pipeline_rule(struct linked_list *token_list)
{
    return command_rule(token_list);    
}

struct AST *command_rule(struct linked_list *token_list)
{
    return simple_command_rule(token_list);
}

struct AST *simple_command_rule(struct linked_list *token_list)
{
    struct AST *tree = NULL;
    struct token *token = list_head(token_list);

    if (token == NULL)
    {
        warnx("Missing word WORD at simple_command_rule");
        return tree;
    }

    if (token->type != WORD)
    {
        warnx("%s: simple_command missmatch", token->symbol);
        free_token(token);
        list_pop(token_list);
        goto simple_command_end;
    }
    
    tree = new_AST(token, COMMAND, NULL);
    list_pop(token_list);

    while (list_head(token_list) != NULL)
    {
        struct AST *child = element_rule(token_list);

        if (child == NULL)
        {
            free_AST(tree);
            tree = NULL;
            goto simple_command_end;
        }

        if (!tree->linked_list)
            tree->linked_list = new_list();

        list_append(tree->linked_list, child);
    }

    simple_command_end:
    return tree;
}

struct AST *element_rule(struct linked_list *token_list)
{
    struct token *token = list_head(token_list);
    struct AST *tree = NULL;

    if (token == NULL)
    {
        warnx("Missing word WORD at element_rule");
        return tree;
    }
    
    if (token->type != WORD)
    {
        warnx("%s: element missmatch", token->symbol);
        free_token(token);
        goto element_end;
    }
    
    tree = new_AST(token, ARG, NULL);

    element_end:
    list_pop(token_list);
    return tree;
}

struct AST *build_shell_AST(struct linked_list *token_list)
{
    struct AST *tree = input_rule(token_list);

    if (!tree)
        return NULL;

    struct AST *sequence = new_AST(NULL, SEQUENCE, new_list());

    list_append(sequence->linked_list, tree);

    return sequence;
}
