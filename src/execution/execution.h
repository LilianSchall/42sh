#ifndef EXECUTION_H
#define EXECUTION_H

#include <err.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "AST/AST.h"
#include "builtin/builtin.h"
#include "garbage_collector/garbage_collector.h"

// #include "exec_echo.h"

// execute the given COMMAND AST
int execute_AST_cmd(struct AST *tree);

// execute the given REDIRECTION AST
int execute_AST_redirection(struct AST *tree);

// execute the given AST
// returns the status code after executing the AST
// returns 127 if a command is unknown
// suffixe traversal
int execute_AST(struct AST *tree);

#endif /* !EXECUTION_H */
