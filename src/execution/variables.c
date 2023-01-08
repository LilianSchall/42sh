#include "variables.h"

static struct linked_list *variables = NULL;

struct var *new_var(char *name, char *value)
{
    struct var *v = malloc(sizeof(struct var));
    if (!v)
        return NULL;
    v->name = strdup(name);
    v->value = strdup(value);
    return v;
}

void free_var(struct var *var)
{
    free(var->name);
    free(var->value);
    free(var);
}

void print_variables(void)
{
    if (!variables)
        return;
    for (struct linked_node *v = variables->head; v; v = v->next)
    {
        struct var *variable = v->data;
        printf("%s: %s\n", variable->name, variable->value);
    }
}

int assign_var(char *name, char *val)
{
    if (!variables)
        variables = new_list();
    for (struct linked_node *v = variables->head; v; v = v->next)
    {
        struct var *variable = v->data;
        if (!strcmp(variable->name, name))
        {
            free(variable->value);
            variable->value = val;
            return 1;
        }
    }
    struct var *n_var = new_var(name, val);
    variables = list_append(variables, n_var);
    return 0;
}

char *get_var(char *name)
{
    if (!variables)
        return NULL;
    for (struct linked_node *v = variables->head; v; v = v->next)
    {
        struct var *variable = v->data;
        if (!strcmp(variable->name, name))
        {
            return variable->value;
        }
    }
    return NULL;
}
