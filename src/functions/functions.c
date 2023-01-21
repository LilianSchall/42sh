#include "functions.h"

struct function
{
    char *name;
    struct AST *ast;
};

struct function *new_function(char *name, struct AST *ast)
{
    struct function *fct = mem_malloc(sizeof(struct function));
    fct->name = gc_strdup(name);
    fct->ast = ast;
    return fct;
}

void free_function(void *data)
{
    struct function *fct = data;
    mem_free(fct->name);
    mem_free(fct);
}

void add_function(struct linked_list *functions, char *name, struct AST *ast)
{
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

int call_function(struct linked_list *functions, char **argv, int *ret_val)
{
    struct linked_node *fun = functions->head;
    for (; fun; fun = fun->next)
    {
        struct function *function = fun->data;
        if (!strcmp(function->name, argv[0]))
        {
            *ret_val = execute_AST(function->ast, argv, functions);
            return 1;
        }
    }
    return 0;
}

void remove_function(struct linked_list *functions, char *name)
{
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
