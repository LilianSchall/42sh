#include "garbage_collector.h"

extern struct garbage_list *garbage_collector;
extern struct garbage_list *new_item(void *ptr, struct garbage_list *next);

char *gc_strdup(char *str)
{
    char *result = strdup(str);

    if (!result || !garbage_collector)
        return result;

    garbage_collector->next = new_item(result, garbage_collector->next);

    return result;
}
