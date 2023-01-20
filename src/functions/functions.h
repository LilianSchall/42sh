#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "AST/AST.h"
#include "execution/execution.h"
#include "linked_list/linked_list.h"

void add_function(char *name, struct AST *ast);
int call_function(char **argv, int *ret_val);
void remove_function(char *name);

#endif /* !FUNCTIONS_H */
