#ifndef AST_H
#define AST_H

#include <stddef.h>

#include "token/token.h"
#include "linked_list/linked_list.h"

enum AST_type
{
  COMMAND,
  ARG,
  SEQUENCE,
  CONDITION, // if, elif, else clause
};

struct AST
{
  struct token *value;
  enum AST_type type;
  struct linked_list *linked_list;
};

// create a tree with the right attributes and returns it.
// returns NULL if value is NULL (wrong execution).
struct AST *new_AST(struct token *value, enum AST_type type, 
        struct linked_list *linked_list);

// free a every node of an AST tree
// if tree is NULL, does nothing
void free_AST(void *data);

#endif /* !AST_H */
