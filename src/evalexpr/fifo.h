#ifndef FIFO_H
#define FIFO_H

#include <stddef.h>

#include "evalexpr_ast.h"
#include "garbage_collector/garbage_collector.h"
#include "lifo.h"

struct fifo
{
    struct list *head;
    struct list *tail;
    size_t size;
};

struct fifo *fifo_init(void);
size_t fifo_size(struct fifo *fifo);
void fifo_push(struct fifo *fifo, struct node *elt);
struct node *fifo_head(struct fifo *fifo);
struct node *fifo_pop(struct fifo *fifo);
void fifo_clear(struct fifo *fifo);
void fifo_destroy(struct fifo *fifo);
void fifo_print(const struct fifo *fifo);

#endif /* !FIFO_H */
