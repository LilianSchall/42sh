#include "lifo.h"

struct list *init_list(struct node *elt)
{
    struct list *l = mem_malloc(sizeof(struct list));
    l->next = NULL;
    l->data = elt;
    return l;
}

struct lifo *lifo_init(void)
{
    struct lifo *f = mem_malloc(sizeof(struct lifo));
    f->head = NULL;
    f->size = 0;
    return f;
}

void lifo_destroy(struct lifo *lifo)
{
    lifo_clear(lifo);
    mem_free(lifo);
}

size_t lifo_size(struct lifo *lifo)
{
    return lifo->size;
}

void lifo_push(struct lifo *lifo, struct node *elt)
{
    struct list *list = init_list(elt);
    if (!lifo->size)
    {
        lifo->head = list;
    }
    else
    {
        list->next = lifo->head;
        lifo->head = list;
    }
    lifo->size += 1;
}

struct node *lifo_head(struct lifo *lifo)
{
    if (!lifo->head)
        return NULL;
    return lifo->head->data;
}

struct node *lifo_pop(struct lifo *lifo)
{
    if (!lifo || !lifo->size)
        return NULL;
    struct list *tmp;
    tmp = lifo->head;
    lifo->head = tmp->next;
    struct node *node = tmp->data;
    mem_free(tmp);
    lifo->size -= 1;
    return node;
}

void lifo_clear(struct lifo *lifo)
{
    while (lifo->size)
        mem_free(lifo_pop(lifo));
}

void lifo_print(const struct lifo *lifo)
{
    if (!lifo->size)
        return;
    struct list *tmp = lifo->head;
    while (tmp)
    {
        printf("%d, %d\n", tmp->data->value, tmp->data->type);
        tmp = tmp->next;
    }
}
