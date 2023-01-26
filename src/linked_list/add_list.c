#include "linked_list.h"

static struct linked_node *new_node(void *data, struct linked_node *next)
{
    struct linked_node *node = mem_malloc(sizeof(struct linked_node));

    if (node == NULL)
        return NULL;

    node->data = data;
    node->next = next;

    return node;
}

struct linked_list *list_append(struct linked_list *list, void *data)
{
    if (data == NULL || list == NULL)
        return list;

    struct linked_node *tail = new_node(data, NULL);

    if (tail == NULL)
        return list;

    if (list->head == NULL)
    {
        list->head = tail;
        return list;
    }

    struct linked_node *head = list->head;

    while (head->next)
    {
        head = head->next;
    }

    head->next = tail;

    return list;
}

struct linked_list *list_insert(struct linked_list *list, void *data, int index)
{
    if (index < 0)
        return list;

    if (index == 0)
    {
        struct linked_node *new_head = new_node(data, list->head);
        list->head = new_head;
        return list;
    }

    struct linked_node *before = list->head;
    while (before != NULL && index > 1)
    {
        before = before->next;
        index--;
    }

    if (!before)
        return list;

    struct linked_node *node = new_node(data, before->next);
    before->next = node;

    return list;
}

void list_concat(struct linked_list *a, struct linked_list *b)
{
    if (!a || !b)
        return;

    if (!a->head)
    {
        a->head = b->head;
        b->head = NULL;
        free_list(b);
        return;
    }

    struct linked_node *node_a = a->head;

    while (node_a->next)
        node_a = node_a->next;

    node_a->next = b->head;
    b->head = NULL;

    free_list(b);
}
