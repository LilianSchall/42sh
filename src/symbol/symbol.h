#ifndef SYMBOL_H
#define SYMBOL_H

#include "garbage_collector/garbage_collector.h"

struct symbol
{
    char *value;
    bool is_expandable;
};

struct symbol new_symbol(char *value, bool is_expandable);

void free_symbol(void *data);

#endif /* !SYMBOL_H */
