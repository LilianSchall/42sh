#include "fifo.h"

struct fifo *fifo_init(void)
{
    struct fifo *f = malloc(sizeof(struct fifo));
    f->head = NULL;
    f->tail = NULL;
    f->size = 0;
    return f;
}

void fifo_destroy(struct fifo *fifo)
{
    fifo_clear(fifo);
    free(fifo);
}

size_t fifo_size(struct fifo *fifo)
{
    return fifo->size;
}

void fifo_push(struct fifo *fifo, struct node *elt)
{
    struct list *list = init_list(elt);
    if (!fifo->size)
    {
        fifo->head = list;
        fifo->tail = list;
    }
    else
    {
        fifo->tail->next = list;
        fifo->tail = list;
    }
    fifo->size += 1;
}

struct node *fifo_head(struct fifo *fifo)
{
    if (!fifo->head)
        return NULL;
    return fifo->head->data;
}

struct node *fifo_pop(struct fifo *fifo)
{
    if (!fifo || !fifo->size)
        return NULL;
    struct list *tmp;
    if (fifo->size == 1)
    {
        tmp = fifo->head;
        fifo->tail = NULL;
        fifo->head = NULL;
    }
    else
    {
        tmp = fifo->head;
        fifo->head = tmp->next;
    }
    struct node *node = tmp->data;
    free(tmp);
    fifo->size -= 1;
    return node;
}

void fifo_clear(struct fifo *fifo)
{
    while (fifo->size)
        free(fifo_pop(fifo));
}

void fifo_print(const struct fifo *fifo)
{
    if (!fifo->size)
        return;
    struct list *tmp = fifo->head;
    while (tmp)
    {
        printf("%d\n", tmp->data->value);
        tmp = tmp->next;
    }
}
