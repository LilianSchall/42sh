#ifndef VARIABLES_H
#define VARIABLES_H

#include <string.h>
#include "linked_list/linked_list.h"

struct var
{
    char *name;
    char *value;
};

int assign_var (char *var, char *val);

#endif // VARIABLES_H
