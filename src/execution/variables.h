#ifndef VARIABLES_H
#define VARIABLES_H

#include <string.h>
#include <stdio.h>
#include "linked_list/linked_list.h"

struct var
{
    char *name;
    char *value;
};

void free_variables(void);
void init_variables(void);
void print_variables(void);
int assign_var (char *var, char *val);
char *get_var(char *name);
void expand_var(char **str);

#endif // VARIABLES_H
