#include "symbol.h"

struct symbol *new_symbol(char *value, bool is_expandable,
        bool is_single_quoted, bool is_double_quoted)
{
    struct symbol *sym = mem_malloc(sizeof(struct symbol));

    if (!sym)
        return NULL;

    sym->value = value;
    sym->is_expandable = is_expandable;
    sym->is_single_quoted = is_single_quoted;
    sym->is_double_quoted = is_double_quoted;

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

// this function returns an array with one symbol element
struct symbol **new_unique_symbols(char *value, bool is_expandable)
{
    struct symbol *sym = new_symbol(value, is_expandable, false, false);

    if (!sym)
        return NULL;

    struct symbol **symbols = mem_calloc(2, sizeof(struct symbol *));

    symbols[0] = sym;

    return symbols;
}

void free_symbol_array(void *data)
{
    if (!data)
        return;

    struct symbol **values = data;

    for (size_t i = 0; values[i]; i++)
    {
        struct symbol *sym = values[i];

        free_symbol(sym);
    }

    mem_free(values);
}

void print_symbols(struct symbol **symbols)
{
    if (!symbols)
        return;

    for (size_t i = 0; symbols[i]; i++)
    {
        struct symbol *sym = symbols[i];

        printf("%s\n", sym->value);
    } 
}
