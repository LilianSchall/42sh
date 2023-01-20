#include "functions.h"

struct function
{
    char *name;
    struct AST *ast;
};

struct linked_list *functions = NULL;

struct function *new_function(char *name, struct AST *ast)
{
    struct function *fct = mem_malloc(sizeof(struct function));
    fct->name = strdup(name);
    fct->ast = ast;
    return fct;
}

void free_function(struct function *fct)
{
    mem_free(fct->name);
    mem_free(fct);
}

void add_function(char *name, struct AST *ast)
{
    if (!functions)
        functions = new_list();
    struct linked_node *fun = functions->head;
    for (; fun; fun=fun->next)
    {
        struct function *function = fun->data;
        if (!strcmp(function->name, name))
        {
            function->ast = ast;
        }
    }
    functions = list_append(functions, new_function(name, ast));
}

int call_function(char **argv, int *ret_val)
{
    if (!functions)
        return 0;
    struct linked_node *fun = functions->head;
    for (; fun; fun = fun->next)
    {
        struct function *function = fun->data;
        if (!strcmp(function->name, argv[0]))
        {
            *ret_val = execute_AST_main(function->ast);
            return 1;
        }
    }
    return 0;
}

void remove_function(char *name)
{
    if (!functions)
        return;
    struct linked_node *fun = functions->head;
    for (; fun; fun = fun->next)
    {
        struct function *function = fun->data;
        if (!strcmp(function->name, name))
        {
            free_function(function);
            fun->next = fun->next->next;
            return;
        }
    }
}
