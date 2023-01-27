#include "parser.h"

static void stick_correctly_child(struct AST **tree, struct AST **redirect_tree,
                                  struct AST **last_redirect_tree,
                                  struct AST *child)
{
    if (child->type == REDIRECTION || child->type == HERE_DOC)
    {
        // we need to append the child to the latest redirection

        if (*redirect_tree == NULL)
            *redirect_tree = child;
        if (*last_redirect_tree != NULL)
        {
            list_insert((*last_redirect_tree)->linked_list, child, 1);
        }

        *last_redirect_tree = child;
    }
    else
    {
        if (*tree == NULL)
        {
            *tree = new_AST(NULL, SEQUENCE, new_list());
        }
        // the child is a word or an assignment
        // so it is an argument
        list_append((*tree)->linked_list, child);
    }
}

static bool parse_prefix(struct linked_list *token_list, struct AST **tree,
                         struct AST **redirect_tree,
                         struct AST **last_redirect_tree)
{
    struct token *token = list_head(token_list);

    if (token == NULL)
        return false;
    // parsing the possible prefix
    while (token != NULL
           && (token->type == IO_NUMBER || is_redirect(token)
               || token->type == VARASSIGNMENT))
    {
        struct AST *child = prefix_rule(token_list, false);

        if (child == NULL)
        {
            free_AST(*redirect_tree);
            *redirect_tree = NULL;
        }
        else
            stick_correctly_child(tree, redirect_tree, last_redirect_tree,
                                  child);

        token = list_head(token_list);
    }
    return true;
}

static bool parse_args(struct linked_list *token_list, struct AST **tree,
                       struct AST **redirect_tree,
                       struct AST **last_redirect_tree)
{
    struct token *token = list_head(token_list);

    // parsing arguments
    while (token != NULL
           && (is_non_delimitator(token->type)
               || is_substitution_ruled(token->type) || token->type == IO_NUMBER
               || is_redirect(token)))
    {
        struct AST *child = element_rule(token_list, false);

        if (child == NULL)
        {
            free_AST(*tree);
            free_AST(*redirect_tree);
            *tree = NULL;
            return false;
        }

        if (!(*tree)->linked_list)
            (*tree)->linked_list = new_list();

        stick_correctly_child(tree, redirect_tree, last_redirect_tree, child);

        // make a look ahead of 1 to know when to stop
        token = list_head(token_list);
    }

    return true;
}

struct AST *simple_command_rule(struct linked_list *token_list,
                                bool trigger_warn)
{
    struct AST *tree = NULL;

    struct AST *redirect_tree = NULL;
    struct AST *last_redirect_tree = NULL;

    if (!parse_prefix(token_list, &tree, &redirect_tree, &last_redirect_tree))
    {
        if (trigger_warn)
            warnx("Missing word WORD at simple_command_rule");
        return NULL;
    }

    struct token *token = list_head(token_list);

    if (!token || !is_substitution_ruled(token->type))
    {
        if (redirect_tree) // then simple_command will be prefix {prefix}
        {
            list_insert(last_redirect_tree->linked_list,
                        new_AST(NULL, SEQUENCE, NULL), 1);
            return redirect_tree;
        }
        else if (tree)
        {
            // then simple command will only be an assignment
            return tree;
        }
        if (trigger_warn)
            warnx("simple_command missmatch");
        goto simple_command_end;
    }
    // now we are parsing the name of the command
    free_AST(tree); // we are erasing all assignment child
    tree = new_AST(NULL, COMMAND, new_list());

    struct AST *name = substitution_rule(token_list, trigger_warn);
    if (!name)
    {
        free_AST(tree);
        free_AST(redirect_tree);
        tree = NULL;
        goto simple_command_end;
    }
    list_append(tree->linked_list, name);
    // the name is now transformed as arg of command AST
    if (!parse_args(token_list, &tree, &redirect_tree, &last_redirect_tree))
    {
        if (trigger_warn)
            warnx("Missing element WORD at simple_command_rule");
        goto simple_command_end;
    }
    // as we have command, we should remove every child
    // that are assignments
    // remove_AST(tree, ASSIGNMENT);
    if (redirect_tree)
    {
        // it means we have a redirection
        // so we insert the tree into the last redirection
        list_insert(last_redirect_tree->linked_list, tree, 1);

        return redirect_tree;
    }

simple_command_end:
    return tree;
}
