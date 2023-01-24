#include "execution/execution.h"


int check_case(char *word, struct AST *ast_case, struct env *env)
{
    int argc = 0;
    char **temp_argv = new_argv(ast_case, &argc, env);

    int ret_val = 0;
    int i = 0;

    while(temp_argv[i] != NULL && ret_val == 0)
    {
        ret_val = !strcmp(word, temp_argv[i]);
        i++;
    }

    free_argv(temp_argv);
    return ret_val;
}


int execute_AST_case(struct AST *tree, struct env *env)
{
    struct linked_node *list = tree->linked_list->head;
    struct AST *ast_val = list->data;
    struct AST *ast_case;

    int ret_val = 0;
    int argc = 0;
    char **argv = new_argv(ast_val, &argc, env);
    char *word = argv[0];

    list = list->next;

    while(list != NULL)
    {
        ast_case = list->data;
        if (ast_case->type == SEQUENCE)
        {
            ret_val = execute_AST_main(ast_case, env);
            break;
        }
        if(check_case(word, ast_case, env))
        {
            list = list->next;
            ret_val = execute_AST_main(list->data, env);
            break;
        }
        list = list->next;
        list = list->next;
    }

    free_argv(argv);
    return ret_val;
}