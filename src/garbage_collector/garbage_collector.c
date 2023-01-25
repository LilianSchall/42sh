#include "garbage_collector.h"

#include <stdio.h>

struct garbage_list *garbage_collector = NULL;

struct garbage_list *new_item(void *ptr, struct garbage_list *next)
{
    struct garbage_list *item = malloc(sizeof(struct garbage_list));

    if (!item)
        return NULL;

    item->ptr = ptr;
    item->next = next;

    return item;
}

static struct garbage_list *search_parent(void *ptr)
{
    struct garbage_list *head = garbage_collector;

    while (head != NULL && head->next != NULL && head->next->ptr != ptr)
    {
        head = head->next;
    }

    if (head == NULL || head->next == NULL)
        return NULL;

    return head;
}

void new_garbage_collector(void)
{
    garbage_collector = new_item(NULL, NULL);
}

static void *mem_alloc(size_t nmemb, size_t size, bool is_calloc)
{
    void *ptr = NULL;

    if (is_calloc)
        ptr = calloc(nmemb, size);
    else
        ptr = malloc(nmemb * size);

    if (!ptr)
        return ptr;

    garbage_collector->next = new_item(ptr, garbage_collector->next);

    return ptr;
}

void *mem_malloc(size_t size)
{
    if (garbage_collector == NULL)
        return malloc(size);

    if (size == 0)
        return NULL;

    void *ptr = mem_alloc(size, 1, false);

    return ptr;
}

void *mem_calloc(size_t nmemb, size_t size)
{
    if (nmemb * size == 0)
        return NULL;

    if (garbage_collector == NULL)
        return calloc(nmemb, size);

    void *ptr = mem_alloc(nmemb, size, true);

    return ptr;
}

void *mem_realloc(void *ptr, size_t size)
{
    if (garbage_collector == NULL)
        return realloc(ptr, size);

    if (size == 0)
    {
        mem_free(ptr);
        return NULL;
    }
    if (ptr == NULL)
        return mem_malloc(size);

    struct garbage_list *parent = search_parent(ptr);

    if (parent == NULL)
        return mem_malloc(size);

    struct garbage_list *node = parent->next;

    void *new_ptr = realloc(ptr, size);

    node->ptr = new_ptr;

    return new_ptr;
}

void mem_free(void *ptr)
{
    if (garbage_collector == NULL)
    {
        free(ptr);
        return;
    }

    if (ptr == NULL)
        return;

    struct garbage_list *parent = search_parent(ptr);

    if (parent == NULL)
    {
        free(ptr);
        return;
    }

    struct garbage_list *node = parent->next;
    parent->next = node->next;

    free(node->ptr);
    free(node);
}

void free_garbage_collector(void)
{
    if (!garbage_collector)
        return;

    struct garbage_list *head = garbage_collector;
    while (head)
    {
        struct garbage_list *next = head->next;

        if (head->ptr)
            free(head->ptr);
        free(head);

        head = next;
    }
}
