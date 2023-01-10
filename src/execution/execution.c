#include "execution.h"

int not_builtin_fn(int argc, char **argv)
{
    if (argc == 0)
        return 0;

    int ret_val = 0;
    int pid = fork();

    if (!pid)
        execvp(argv[0], argv);

    ret_val = 0;
    wait(&ret_val);

    return ret_val;
}

int execute_AST_cmd(struct AST *tree)
{
    char *cmd = tree->value->symbol;
    int ret_val = 0;

    struct linked_list *ll_word = get_linked_list_from_AST(tree);
    int argc = 0;
    char **argv = new_argv(ll_word, &argc);

    if (!strcmp("echo", cmd)) // builtin command
    {
        ret_val = echo_fn(argc, argv);
    }
    else if (!strcmp("true", cmd)) // true
    {
        ret_val = true_fn(argc, argv);
    }
    else if (!strcmp("false", cmd)) // false
    {
        ret_val = false_fn(argc, argv);
    }
    else
    {
        ret_val = not_builtin_fn(argc, argv); // not a builtin command
    }

    free_argv(argc, argv);
    free_list(ll_word);

    return ret_val;
}

int execute_AST_if(struct AST *tree)
{
    struct linked_node *child = tree->linked_list->head;
    struct AST *cond = child->data;
    child = child->next;
    struct AST *t = child->data;
    if (execute_AST(cond) == 0) // cond is true, we execute the 'then'
    {
        return execute_AST(t);
    }
    else
    {
        if (child->next) // cond is false, we execute the 'else'
            return execute_AST(child->next->data);
    }
    return 0;
}


// exec a while or until command
// if val_cond = 0 -> while
// if val_cond = 1 -> until
int execute_AST_while_until(struct AST *tree, int val_cond)
{
    int return_val = 0;
    int while_cond = val_cond;

    struct linked_node *child = tree->linked_list->head;
    struct AST *cond = child->data;

    child = child->next;
    struct AST *bloc = child->data;

    while (while_cond == val_cond)
    {
        while_cond = execute_AST(cond); // check condition

        if (while_cond == val_cond)
            return_val = execute_AST(bloc); // exec commands
    }    

    return return_val;
}

int execute_AST_for(struct AST *tree)
{
    int ret_val = 0;
    struct linked_node *child = tree->linked_list->head;
    struct AST *ast_arg = child->data;
    char *var_name = ast_arg->value->symbol;
    child = child->next; // should not be NULL (check here if error occurs)
    struct AST *ast_iter_seq = child->data;
    child = child->next; // should not be NULL either
    struct AST *ast_seq = child->data;
    if (ast_iter_seq->type == ITER)
    {
        struct linked_node *iter_child = ast_iter_seq->linked_list->head;
        while (iter_child)
        {
            struct AST *iter_arg = iter_child->data;
            assign_var(var_name, iter_arg->value->symbol);
            ret_val = execute_AST(ast_seq);
            iter_child = iter_child->next;
        }
    }
    else // the tree is a SEQUENCE, need to exec in a subshell
    {
        // TODO in step 3
    }
    return ret_val;
}

int execute_AST_operator(struct AST *tree)
{
    char *op = tree->value->symbol;
    int ret_val = 0;
    int ret_val2 = 0;

    struct linked_node *node = tree->linked_list->head;
    struct AST *child = node->data;
    struct AST *child2;

    if (node->next)
        child2 = node->next->data;

    if (!strcmp("!", op)) // ! condition
    {
        ret_val = ! execute_AST(child);
    }
    else if (!strcmp("&&", op)) // && condition
    {
        ret_val = execute_AST(child);
        ret_val2 = execute_AST(child2);

        if(ret_val == 0 && ret_val2 == 0)
            ret_val = 0;
        else
            ret_val = 1;
    }
    else if (!strcmp("||", op)) // || condition
    {
        ret_val = execute_AST(child);
        ret_val2 = execute_AST(child2);

        if(ret_val == 1 && ret_val2 == 1)
            ret_val = 1;
        else
            ret_val = 0;
    }


    return ret_val;
}

int execute_AST_assignment(struct AST *tree)
{
    int ret_val = 1;
    struct linked_node *child = tree->linked_list->head;
    struct AST *var_name_ast = child->data;
    char *var_name = var_name_ast->value->symbol; // variable name is the token value of the ast
    struct AST *var_value_ast = child->next->data; // taking second child(cant be NULL)
    
    if (var_value_ast->type == ARG)
    {
        ret_val = assign_var(var_name, var_value_ast->value->symbol);
    }
    else // the child is a sequence -> subshell and take stdout as value
    {
        //TODO in step 3 or 4
    }
    return ret_val;
}

int execute_AST(struct AST *tree)
{
    if (!tree)
        return 0;

    int ret_val = 0;

    for (struct linked_node *node = tree->linked_list->head; node;
         node = node->next)
    {
        struct AST *child = node->data;
        switch (child->type)
        {
        case COMMAND:
            ret_val = execute_AST_cmd(child);
            break;
        case SEQUENCE:
            ret_val = execute_AST(child);
            break;
        case OPERATOR:
            ret_val = execute_AST_operator(child);
            break;
        case CONDITION: {
            switch (child->value->type)
            {
            case IF:
                ret_val = execute_AST_if(child);
                break;
            case WHILE:
                ret_val = execute_AST_while_until(child, 0); // while is true
                break;
            case UNTIL:
                ret_val = execute_AST_while_until(child, 1); // until is true
                break;
            case FOR:
                ret_val = execute_AST_for(child);
                break;
            default:
                break;
            }
        }
            break;
        case ASSIGNMENT:
            ret_val = execute_AST_assignment(child);
            break;
        default:
            break;
        }
    }
    return ret_val;
}
