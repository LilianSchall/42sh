#ifndef SYMBOL_H
#define SYMBOL_H

#include <stddef.h>

#include "garbage_collector/garbage_collector.h"

struct symbol
{
    char *value;
    bool is_expandable;
};

struct symbol new_symbol(char *value, bool is_expandable);

struct symbol **new_unique_symbols(char *value, bool is_expandable);

void free_symbol(void *data);

void free_symbol_array(void *data);

void print_symbols(struct symbol **values);

#endif /* !SYMBOL_H */
