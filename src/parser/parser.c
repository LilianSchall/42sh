#include "parser.h"

static struct AST *input_rule(struct linked_list *token_list);
static struct AST *list_rule(struct linked_list *token_list);
static struct AST *and_or_rule(struct linked_list *token_list, bool trigger_warn);
static struct AST *pipeline_rule(struct linked_list *token_list, bool trigger_warn);
static struct AST *command_rule(struct linked_list *token_list, bool trigger_warn);
static struct AST *simple_command_rule(struct linked_list *token_list, bool trigger_warn);
static struct AST *element_rule(struct linked_list *token_list, bool trigger_warn);
static struct AST *shell_command_rule(struct linked_list *token_list, bool trigger_warn);
static struct AST *rule_if_rule(struct linked_list *token_list, bool trigger_warn);
static struct AST *else_clause_rule(struct linked_list *token_list, bool trigger_warn);
static struct AST *compound_list_rule(struct linked_list *token_list, bool trigger_warn);

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
    struct AST *child = and_or_rule(token_list, true);
    
    if (child == NULL)
        return NULL;

    struct AST *tree = new_AST(NULL, SEQUENCE, new_list());

    list_append(tree->linked_list, child);
    
    struct token *token = list_head(token_list);
    while (token != NULL && token->type == SEMICOLON)
    {
        free_token(token);
        list_pop(token_list);

        // check for and_or rule again

        child = and_or_rule(token_list, false);

        if (child == NULL)
            break;

        list_append(tree->linked_list, child);
        token = list_head(token_list);
    }

    return tree;
}

struct AST *and_or_rule(struct linked_list *token_list, bool trigger_warn)
{
    return pipeline_rule(token_list, trigger_warn);
}

struct AST *pipeline_rule(struct linked_list *token_list, bool trigger_warn)
{
    return command_rule(token_list, trigger_warn);    
}

struct AST *command_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *token = list_head(token_list);
    
    if (token == NULL)
    {
        if (trigger_warn)
            warnx("Missing token at command_rule");
        return NULL;
    }
    
    if (token->type == WORD)
        return simple_command_rule(token_list, trigger_warn);
    else if (token->type == IF)
        return shell_command_rule(token_list, trigger_warn);
    //else
    if (trigger_warn)
        warnx("No match for token: %s at command_rule", token->symbol);
    return NULL;
}

struct AST *simple_command_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct AST *tree = NULL;
    struct token *token = list_head(token_list);

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
    while (token != NULL && token->type == WORD)
    {
        struct AST *child = element_rule(token_list, trigger_warn);

        if (child == NULL)
        {
            free_AST(tree);
            tree = NULL;
            goto simple_command_end;
        }

        if (!tree->linked_list)
            tree->linked_list = new_list();

        list_append(tree->linked_list, child);
        token = list_head(token_list);
    }

    simple_command_end:
    return tree;
}

struct AST *element_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *token = list_head(token_list);
    struct AST *tree = NULL;

    if (token == NULL)
    {
        if (trigger_warn)
            warnx("Missing word WORD at element_rule");
        return tree;
    }
    
    if (token->type != WORD)
    {
        if (trigger_warn)
            warnx("%s: element missmatch", token->symbol);
        goto element_end;
    }
    
    tree = new_AST(token, ARG, NULL);
    list_pop(token_list);
    
    element_end:
    return tree;
}

struct AST *shell_command_rule(struct linked_list *token_list, bool trigger_warn)
{
    return rule_if_rule(token_list, trigger_warn);
}

struct token *consume_token(struct linked_list *token_list, 
        enum token_type type, struct log_info info)
{
    struct token *token = list_head(token_list);

    if (!token)
    {
        if (info.trigger_warn)
            warnx("Missing word %s at %s", info.sym, info.rulename);
        return NULL;
    }

    if (token->type != type)
    {
        if (info.trigger_warn)
            warnx("%s: %s missmatch at %s", token->symbol, info.sym, 
                    info.rulename);
        return NULL;
    }

    list_pop(token_list);

    return token;
}

struct AST *combine_token_AST(struct AST *tree, struct token *token)
{
    if (tree == NULL)
        return NULL;

    tree->value = token;
    return tree;
}

struct AST *rule_if_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *if_token = NULL;
    struct token *then_token = NULL;
    struct token *tmp = NULL;
    // parsing if token
    struct log_info info = { .sym = "IF", .rulename = "rule_if_rule", 
        .trigger_warn = true };

    if (!(if_token = consume_token(token_list, IF, info)))
        return NULL;

    struct AST *cond = compound_list_rule(token_list, trigger_warn);

    if (cond == NULL)
        return NULL;

    // parsing then token
    info.sym = "THEN";
    if (!(then_token = consume_token(token_list, THEN, info)))
    {
        free_token(if_token);
        free_AST(cond);
        return NULL;
    }

    struct AST *then_clause = compound_list_rule(token_list, trigger_warn);
    if (!then_clause)
    {
        free_token(if_token);
        free_token(then_token);
        free_AST(cond);
        return NULL;
    }

    then_clause = combine_token_AST(then_clause, then_token);

    struct AST *else_clause = else_clause_rule(token_list, false);

    // parsing fi token
    info.sym = "FI";
    if (!(tmp = consume_token(token_list, FI, info)))
    {
        free_token(if_token);
        free_AST(cond);
        free_AST(then_clause);
        free_AST(else_clause);
        return NULL;
    }
    free_token(tmp);


    struct AST *tree = new_AST(if_token, CONDITION, new_list());
    list_append(tree->linked_list, cond);
    list_append(tree->linked_list, then_clause);
    list_append(tree->linked_list, else_clause);

    return tree;
}

struct AST *else_clause_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct log_info info = { .sym = "ELSE", .rulename = "else_clause_rule",
        .trigger_warn = trigger_warn };

    struct token *else_token = consume_token(token_list, ELSE, info);

    if (else_token)
    {
        struct AST *tree = compound_list_rule(token_list, trigger_warn);
        
        if (!tree)
            return NULL;
        return combine_token_AST(tree, else_token);
    }
    info.sym = "ELIF";
    struct token *elif_token = consume_token(token_list, ELIF, info);

    if (!elif_token)
        return NULL;
    free_token(elif_token);

    struct AST *cond = compound_list_rule(token_list, trigger_warn);

    if (!cond)
        return NULL;

    info.sym = "THEN";
    struct token *then_token = consume_token(token_list, THEN, info);
    if (!then_token)
    {
        free_AST(cond);
        return NULL;
    }

    struct AST *then_clause = compound_list_rule(token_list, trigger_warn);

    if (!then_clause)
    {
        free_AST(cond);
        free_token(then_token);
        return NULL;
    }
    then_clause = combine_token_AST(then_clause, then_token);

    struct AST *else_clause = else_clause_rule(token_list, false);

    struct AST *tree = new_AST(new_token(strdup("else"), ELSE), SEQUENCE, new_list());
    struct AST *if_tree = new_AST(new_token(strdup("if"), IF), CONDITION, new_list());

    list_append(tree->linked_list, if_tree);
    list_append(if_tree->linked_list, cond);
    list_append(if_tree->linked_list, then_clause);
    list_append(if_tree->linked_list, else_clause); // if it is null, it will do nothing

    return tree;
}

struct AST *compound_list_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct log_info info = { .sym = "\\n", .rulename = "compound_list_rule",
        .trigger_warn = false };
    struct token *newline_token = NULL;
    while ((newline_token = consume_token(token_list, NEWLINE, info)) != NULL)
        free_token(newline_token);

    
    struct AST *tree = new_AST(NULL, SEQUENCE, new_list());
    struct AST *command = and_or_rule(token_list, trigger_warn);

    if (!command)
        return NULL;

    list_append(tree->linked_list, command);

    while (true)
    {
        info.sym = ";";
        struct token *tmp = consume_token(token_list, SEMICOLON, info);

        if (!tmp)
        {
            info.sym = "\\n";
            tmp = consume_token(token_list, NEWLINE, info);

            if (!tmp)
                break;
        }
        free_token(tmp);
        info.sym = "\\n";
        while ((tmp = consume_token(token_list, NEWLINE, info)) != NULL)
            free_token(tmp);

        command = and_or_rule(token_list, false);

        if (!command)
            break;

        list_append(tree->linked_list, command);
    }

    info.sym = ";";
    struct token *tmp = consume_token(token_list, SEMICOLON, info);

    if (tmp)
        free_token(tmp);

    info.sym = "\\n";
    while ((tmp = consume_token(token_list, NEWLINE, info)) != NULL)
        free_token(tmp);

    return tree;
}

struct AST *build_shell_AST(struct linked_list *token_list)
{
    struct AST *tree = input_rule(token_list);

    if (!tree)
        return NULL;
    
    if (tree->type == SEQUENCE)
        return tree;

    struct AST *sequence = new_AST(NULL, SEQUENCE, new_list());

    list_append(sequence->linked_list, tree);

    return sequence;
}
