#include "parser.h"

static struct AST *create_for_tree(struct token *for_token, struct token *word,
        struct AST *iter, struct AST *seq)
{
    struct AST *for_tree = new_AST(for_token, CONDITION, new_list());
    list_append(for_tree->linked_list, new_AST(word, ARG, NULL));
    list_append(for_tree->linked_list, iter);
    list_append(for_tree->linked_list, seq);

    return for_tree;
}


// this function shall parse the [';']
// if this function is 
static struct AST *parsing_semicolon(struct linked_list *token_list)
{
    struct token *token = list_head(token_list);
    struct AST *tree = NULL;
    if (!token)
        return NULL;

    if (token->type == SEMICOLON)
    {
        list_pop(token_list);
        free_token(token);
        goto create_iter;
    }

    for (struct linked_node *node = token_list->head; node; node = node->next)
    {
        token = node->data;

        if (token->type == NEWLINE)
            continue;
        else if (token->type == DO)
            goto create_iter;
        else // we possibly encounter an IN token
            return NULL;
    }

create_iter:
    tree = new_AST(NULL, ITER, new_list());

    list_append(tree->linked_list, new_AST(new_token(strdup("$@"), WORD), ARG, NULL));

    return tree;
}

// this function shall parse the [ {'\n' 'in' { WORD } ';' | '\n' ]
// if there is no word WORD, the function shall return an ITER AST with no child in it
static struct AST *parsing_in_clause(struct linked_list *token_list)
{
    purge_newline_token(token_list);

    struct token *token =NULL;
    struct log_info infos = { .trigger_warn = true,
        .sym = "IN", .rulename = "rule_for_rule" };

    if (!(token = consume_token(token_list, IN, infos)))
    {
        return NULL;
    }
    free_token(token); // in token is useless now

    struct AST *tree = new_AST(NULL, ITER, new_list());
    
    infos.sym = "WORD";
    infos.trigger_warn = false;
    while((token = consume_token(token_list, WORD, infos)) != NULL)
    {
        list_append(tree->linked_list, new_AST(token, ARG, NULL));
    }

    // parsing ';' or '\n'
    token = list_head(token_list);
    if (token->type != NEWLINE && token->type != SEMICOLON)
    {
        free_AST(tree);
        return NULL;
    }
    list_pop(token_list);
    free_token(token);

    return tree;
}

struct AST *rule_for_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *for_token = NULL;
    struct token *word = NULL;
    struct token *do_token = NULL;

    struct log_info infos = { .trigger_warn = trigger_warn,
        .sym = "FOR", .rulename = "rule_for_rule" };

    if (!(for_token = consume_token(token_list, FOR, infos)))
        goto for_error;

    infos.sym = "WORD";
    if (!(word = consume_token(token_list, WORD, infos)))
    {
        goto for_error;
    }
    
    // now we have to parse [';'] | [ {'\n'} 'in' { WORD } ';' | '\n' ]
    
    struct AST *iter = parsing_semicolon(token_list);

    if (!iter)
    {
        iter = parsing_in_clause(token_list);

        if (!iter)
        {
            if (trigger_warn)
                warnx("missing iterator on rule_for_rule");
            goto for_error;
        }
    }

    // now we can purge any \n
    purge_newline_token(token_list);

    // parsing do
    infos.sym = "DO";
    if (!(do_token = consume_token(token_list, DO, infos)))
        goto for_error;
    free_token(do_token);

    // parsing compount_list
    struct AST *seq = compound_list_rule(token_list, trigger_warn);

    if (!seq)
        goto free_tree;

    // parsing done
    infos.sym = "DONE";
    struct token *done_token = NULL;

    if (!(done_token = consume_token(token_list, DONE, infos)))
        goto free_tree;
    free_token(done_token);

    return create_for_tree(for_token, word, iter, seq);

free_tree:
    free_AST(iter);
    free_AST(seq);

    for_error:
    free_token(do_token);
    free_token(word);
    free_token(for_token);
    return NULL;
}
