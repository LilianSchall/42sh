#ifndef EXECUTION_H
#define EXECUTION_H

#include <err.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "AST/AST.h"
#include "builtin/builtin.h"
#include "garbage_collector/garbage_collector.h"

// execute ast d_subshell
// return the string printed in stdout of the subshell
char *execute_AST_D_SUBSHELL(struct AST *tree);

// execute the given AST
// returns the status code after executing the AST
// returns 127 if a command is unknown
// suffixe traversal
int execute_AST(struct AST *tree);

#endif /* !EXECUTION_H */

