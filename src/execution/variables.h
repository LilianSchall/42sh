#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdio.h>
#include <string.h>

#include "linked_list/linked_list.h"

struct var
{
    char *name;
    char *value;
};

void print_variables(void);
void free_variables(void);

int assign_var(char *name, char *val);

char *get_var(char *name);
void replace_var(char **str);

#endif // VARIABLES_H
