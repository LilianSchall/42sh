#ifndef SYMBOL_H
#define SYMBOL_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "garbage_collector/garbage_collector.h"

struct symbol
{
    char *value;
    bool is_expandable;
    bool is_single_quoted;
    bool is_double_quoted;
};

struct symbol *new_symbol(char *value, bool is_expandable,
                          bool is_single_quoted, bool is_double_quoted);

struct symbol **new_unique_symbols(char *value, bool is_expandable,
                                   bool is_single_quoted,
                                   bool is_double_quoted);

void free_symbol(void *data);

void free_symbol_array(void *data);

void print_symbols(struct symbol **values);

char *get_cat_symbols(struct symbol **values);

bool is_quoted(struct symbol *sym);

#endif /* !SYMBOL_H */
