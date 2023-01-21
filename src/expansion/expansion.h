#ifndef EXPANSION_H
#define EXPANSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "symbol/symbol.h"
#include "garbage_collector/garbage_collector.h"

#define RAND_MAX 32767

struct var
{
    int argc;
    char **argv;
};

char *expand_var(const char *str, char **argv, int quoted);
char *expand_symbol_array(struct symbol **values, char **argv);

#endif /* !EXPANSION_H */

