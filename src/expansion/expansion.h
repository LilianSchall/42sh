#ifndef EXPANSION_H
#define EXPANSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "symbol/symbol.h"
#include "garbage_collector/garbage_collector.h"

struct var
{
    int argc;
    char **argv;
};

void set_spec_var(int argc, char **argv);
char *expand_var(const char *str, int quoted);
char *expand_symbol_array(struct symbol **values);

#endif /* !EXPANSION_H */

