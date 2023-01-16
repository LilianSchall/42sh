#include "symbol.h"

struct symbol *new_symbol(char *value, bool is_expandable)
{
    struct symbol *sym = mem_malloc(sizeof(struct symbol));

    if (!sym)
        return NULL;

    sym->value = value;
    sym->is_expandable = is_expandable;

    return sym;
}

void free_symbol(void *data)
{
    if (!data)
        return;

    struct symbol *sym = data;

    mem_free(sym->value);
    mem_free(sym);
}

