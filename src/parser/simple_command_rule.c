#include "parser.h"

static void stick_correctly_child(struct AST **tree,
        struct AST **redirect_tree, 
        struct AST **last_redirect_tree,
        struct AST *child)
{
   if (child->type == REDIRECTION)
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

struct AST *simple_command_rule(struct linked_list *token_list,
                                bool trigger_warn)
{
    struct AST *tree = NULL;
    struct AST *command_tree = NULL;
    struct token *token = list_head(token_list);
    
    struct AST *redirect_tree = NULL;
    struct AST *last_redirect_tree = NULL;

    if (token == NULL)
    {
        if (trigger_warn)
            warnx("Missing word WORD at simple_command_rule");
        return tree;
    }
    // parsing the possible prefix
    while (token != NULL && (token->type == IO_NUMBER 
                || is_redirect(token) || token->type == VARASSIGNMENT))
    {
        struct AST *child = prefix_rule(token_list, trigger_warn);

        if (child == NULL)
        {
            free_AST(redirect_tree);
        }
        
        stick_correctly_child(&tree, &redirect_tree, 
                &last_redirect_tree, child);

        token = list_head(token_list);
    }

    if (!token || token->type != WORD)
    {
        if (redirect_tree) // then simple_command will be prefix {prefix}
        {
            list_insert(last_redirect_tree->linked_list, new_AST(NULL, SEQUENCE, NULL), 1);
            return redirect_tree;
        }
        else if (tree)
        {
            // then simple command will only be an assignment
            return tree;
        }
        if (trigger_warn)
            warnx("%s: simple_command missmatch", token->symbol);
        goto simple_command_end;
    }
    
    free_AST(tree); // we are erasing all assignment child
    tree = new_AST(token, COMMAND, NULL);
    list_pop(token_list);

    token = list_head(token_list);
    while (token != NULL && (token->type == WORD ||
           token->type == IO_NUMBER || is_redirect(token)))
    {
        struct AST *child = element_rule(token_list, trigger_warn);

        if (child == NULL)
        {
            free_AST(tree);
            free_AST(redirect_tree);
            tree = NULL;
            goto simple_command_end;
        }

        if (!tree->linked_list)
            tree->linked_list = new_list();

        stick_correctly_child(&tree, &redirect_tree,
                &last_redirect_tree, child);

        // make a look ahead of 1 to know when to stop
        token = list_head(token_list);
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
