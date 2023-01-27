#ifndef LIFO_H
#define LIFO_H

#include <stddef.h>

#include "evalexpr_ast.h"

struct list
{
    struct node *data;
    struct list *next;
};

struct lifo
{
    struct list *head;
    size_t size;
};

struct list *init_list(struct node *elt);
struct lifo *lifo_init(void);
size_t lifo_size(struct lifo *lifo);
void lifo_push(struct lifo *lifo, struct node *elt);
struct node *lifo_head(struct lifo *lifo);
struct node *lifo_pop(struct lifo *lifo);
void lifo_clear(struct lifo *lifo);
void lifo_destroy(struct lifo *lifo);
void lifo_print(const struct lifo *lifo);

#endif /* !lifo_H */
