#include "linked_list.h"


struct linked_list *new_list(void)
{
    struct linked_list *list = malloc(sizeof(struct linked_list));

    if (list == NULL)
        return NULL;

    list->head = NULL;

    return list;
}

void *list_head(struct linked_list *list)
{
    if (list == NULL)
        return NULL;

    if (list->head == NULL)
        return NULL;

    return list->head->data;
}

size_t list_size(struct linked_list *list)
{
    if (list == NULL)
        return 0;

    struct linked_node *head = list->head;

    size_t len = 0;

    while (head != NULL)
    {
        head = head->next;
        len++;
    }

    return len;
}

struct linked_list *list_append(struct linked_list *list, void *data)
{
    if (data == NULL || list == NULL)
        return list;

    struct linked_node *tail = malloc(sizeof(struct linked_node));

    if (tail == NULL)
        return list;

    tail->next = NULL;
    tail->data = data;

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

static void free_node(struct linked_node *node, void (*free_fct)(void *))
{
    if (node == NULL)
        return;

    if (free_fct != NULL)
    {
        free_fct(node->data);
    }
    free(node);
}

struct linked_list *list_pop(struct linked_list *list)
{
    if (list == NULL)
        return NULL;

    list = list_pop_non_safe(list);

    return list;
}

struct linked_list *list_pop_non_safe(struct linked_list *list)
{
    if (list->head == NULL)
        return list;

    struct linked_node *new_head = list->head->next;

    free_node(list->head, NULL);

    list->head = new_head;

    return list;
}

static void __list_deep_free(struct linked_list *list, void (*free_fct)(void *))
{
    if (list == NULL)
        return;

    struct linked_node *head = list->head;
    struct linked_node *next = NULL;

    while (head != NULL)
    {
        next = head->next;

        free_node(head, free_fct);

        head = next;
    }

    free(list);
}

void deep_free_list(struct linked_list *list, void (*free_fct)(void *))
{
    __list_deep_free(list, free_fct);
}

void free_list(struct linked_list *list)
{
    __list_deep_free(list, NULL);
}
