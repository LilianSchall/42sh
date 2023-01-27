#include "parser.h"

static struct AST *form_iter(struct linked_list *token_list, bool trigger_warn,
                             struct AST *iter, bool is_default)
{
    struct token *token = list_head(token_list);
    struct AST *word = NULL;
    while (token && token->type == R_PIPE)
    {
        list_pop(token_list);
        free_token(token);
        list_head(token_list);

        token = list_head(token_list);

        if (!token
            || (!is_non_delimitator(token->type)
                && !is_substitution_ruled(token->type)))
        {
            free_AST(iter);
            return NULL;
        }

        if (is_default)
        {
            word = substitution_rule(token_list, trigger_warn);
            free_AST(word);
        }
        else
        {
            char *sym = get_cat_symbols(token->values);

            if (!strcmp(sym, "*"))
            {
                is_default = true;
                list_pop(token_list);
                free_token(token);
                deep_free_list(iter->linked_list, free_AST);
                iter->linked_list = new_list();
            }
            else
                list_append(iter->linked_list,
                            substitution_rule(token_list, trigger_warn));
            mem_free(sym);
        }

        token = list_head(token_list);
    }
    return iter;
}

static struct linked_list *new_case_item_list(struct linked_list *token_list,
                                              struct AST *iter)
{
    struct AST *compound = list_rule(token_list);
    struct linked_list *list = new_list();
    list_append(list, iter);
    list_append(list, compound ? compound : new_AST(NULL, SEQUENCE, NULL));

    return list;
}

static struct linked_list *case_item_rule(struct linked_list *token_list,
                                          bool trigger_warn, int *err)
{
    *err = 0;
    bool is_default = false;
    struct token *token = list_head(token_list);

    if (!token || token->type == ESAC)
        return NULL;

    if (token->type == OPEN_PARENTHESE)
    {
        list_pop(token_list);
        free_token(token);
        token = list_head(token_list);
        *err = 1; // we set it to 1 so that we know that we have parsed 1 item
    }

    if (!token
        || (!is_non_delimitator(token->type)
            && !is_substitution_ruled(token->type)))
        return NULL;

    char *sym = get_cat_symbols(token->values);

    if (!strcmp(sym, "*") && !(is_quoted(token->values[0])))
    {
        is_default = true;
        list_pop(token_list);
        free_token(token);
    }
    mem_free(sym);

    struct AST *word =
        !is_default ? substitution_rule(token_list, trigger_warn) : NULL;

    if (!word && !is_default)
        return NULL;

    struct AST *iter = new_AST(NULL, ITER, new_list());
    list_append(iter->linked_list, word);

    iter = form_iter(token_list, trigger_warn, iter, is_default);

    if (!iter)
    {
        free_AST(iter);
        return NULL;
    }

    token = list_head(token_list);

    if (!token || token->type != CLOSE_PARENTHESE)
    {
        free_AST(iter);
        return NULL;
    }
    list_pop(token_list);
    free_token(token);

    purge_newline_token(token_list);
    *err = 0;
    return new_case_item_list(token_list, iter);
}

static struct AST *case_clause_rule(struct linked_list *token_list,
                                    bool trigger_warn, int *err)
{
    struct linked_list *case_item =
        case_item_rule(token_list, trigger_warn, err);

    if (!case_item)
    {
        *err = 1;
        return NULL;
    }

    struct AST *tree = new_AST(NULL, CASE_C, new_list());
    list_concat(tree->linked_list, case_item);
    struct token *token = list_head(token_list);

    while (token && token->type == DOUBLE_SEMICOLON)
    {
        list_pop(token_list);
        free_token(token);

        purge_newline_token(token_list);

        case_item = case_item_rule(token_list, trigger_warn, err);

        if (*err)
        {
            free_AST(tree);
            return NULL;
        }

        if (!case_item)
            break;

        list_concat(tree->linked_list, case_item);

        token = list_head(token_list);
    }

    return tree;
}

static struct AST *parse_case_clause(struct linked_list *token_list,
                                     struct log_info info, struct AST *word,
                                     int *err)
{
    struct token *token = list_head(token_list);
    info.sym = "in";
    if (!(token = consume_token(token_list, IN, info)))
    {
        free_AST(word);
        return NULL;
    }

    free_token(token);
    token = NULL;

    purge_newline_token(token_list);
    struct AST *case_clause = case_clause_rule(token_list, false, err);

    if (*err)
        return NULL;

    if (!case_clause)
        case_clause = new_AST(NULL, CASE_C, new_list());

    info.sym = "esac";
    if (!(token = consume_token(token_list, ESAC, info)))
    {
        free_AST(word);
        return NULL;
    }
    free_token(token);

    list_insert(case_clause->linked_list, word, 0);

    return case_clause;
}

struct AST *rule_case_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *token = NULL;
    struct AST *word = NULL;
    struct log_info info = { .sym = "case",
                             .rulename = "rule_case_rule",
                             .trigger_warn = trigger_warn };
    int err = 0;

    if (!(token = consume_token(token_list, CASE, info)))
        return NULL;

    free_token(token);

    token = list_head(token_list);

    if (!token
        || (!is_non_delimitator(token->type)
            && !is_substitution_ruled(token->type)))
    {
        token = NULL; // unreference token
        goto rule_case_err;
    }

    word = substitution_rule(token_list, trigger_warn);
    token = NULL;
    if (!word)
        goto rule_case_err;
    struct AST *tmp = new_AST(NULL, ITER, new_list());
    list_append(tmp->linked_list, word);
    word = tmp;
    purge_newline_token(token_list);

    struct AST *case_clause = parse_case_clause(token_list, info, word, &err);

    if (!err)
        return case_clause;

rule_case_err:

    if (trigger_warn)
        warnx("Missing word in rule_case_rule");
    free_token(token);
    free_AST(word);
    return NULL;
}
