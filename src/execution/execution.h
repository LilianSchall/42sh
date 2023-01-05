#ifndef EXECUTION_H
#define EXECUTION_H

#include "AST/AST.h"
#include "builtin/builtin.h"


// execute the given AST
// returns the status code after executing the AST
// returns 127 if a command is unknown
// suffixe traversal
int execute_AST(struct AST *tree);

#endif /* !EXECUTION_H */
