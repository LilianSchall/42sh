#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

#include "garbage_collector/garbage_collector.h"

struct linked_node
{
    void *data;
    struct linked_node *next;
};

struct linked_list
{
    struct linked_node *head;
};

struct linked_list *new_list(void);

void *list_head(struct linked_list *list);
size_t list_size(struct linked_list *list);

struct linked_list *list_insert(struct linked_list *list, void *data,
                                int index);
struct linked_list *list_append(struct linked_list *list, void *data);
struct linked_list *list_pop(struct linked_list *list);
struct linked_list *list_pop_non_safe(struct linked_list *list);

void deep_free_list(struct linked_list *list, void (*free_fct)(void *));
void free_list(struct linked_list *list);

#endif /* !LINKED_LIST_H */

