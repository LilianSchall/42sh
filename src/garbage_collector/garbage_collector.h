#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include <err.h>
#include <stdbool.h>
#include <stdlib.h>

struct garbage_list
{
    void *ptr;
    struct garbage_list *next;
};

void new_garbage_collector(void);
void *mem_malloc(size_t size);
void *mem_calloc(size_t nmemb, size_t size);
void *mem_realloc(void *ptr, size_t size);

void mem_free(void *ptr);

void free_garbage_collector(void);

#endif /* GARBAGE_COLLECTOR_H */

