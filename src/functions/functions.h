#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "AST/AST.h"
#include "execution/execution.h"
#include "linked_list/linked_list.h"

void free_function(void *data);
void add_function(struct linked_list *functions, char *name, struct AST *ast);
int call_function(struct linked_list *functions, char **argv, int *ret_val);
int remove_function(struct linked_list *functions, char *name);

#endif /* !FUNCTIONS_H */
