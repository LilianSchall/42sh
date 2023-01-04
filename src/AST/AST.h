#ifndef AST_H
#define AST_H

#include <stddef.h>

#include "token/token.h"

struct AST
{
  struct token *value;
  enum AST_type type;
  struct AST *left_child;
  struct AST *right_child;
};

enum AST_type
{
  COMMAND,
  ARG,
  CONDITION, // if, elif, else clause
};

// create a tree with the right attributes and returns it.
// returns NULL if value is NULL (wrong execution).
struct AST *new_AST(struct token *value, enum AST_type type, 
        struct AST *left_child, struct AST *right_child);

// free a every node of an AST tree
// if tree is NULL, does nothing
void free_AST(struct AST *tree);

#endif /* !AST_H */
