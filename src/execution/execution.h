#ifndef EXECUTION_H
#define EXECUTION_H

#include <err.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "AST/AST.h"
#include "builtin/builtin.h"
#include "functions/functions.h"
#include "garbage_collector/garbage_collector.h"

// execute ast d_subshell
// return the string printed in stdout of the subshell
char *execute_AST_D_SUBSHELL(struct AST *tree, char **argv);

// execute D_SUBSHELL ast $(..)
char *execute_AST_D_SUBSHELL(struct AST *tree, char **argv);

// execute SUBSHELL ast (..)
int execute_AST_subshell(struct AST *tree, char **argv);

// execute ast redirection ( > >> < >& <> >| <& )
int execute_AST_redirection(struct AST *tree, char **argv);

// execute the ast condition (for while until if)
int execute_AST_condition(struct AST *tree, char **argv);

// execute the ast assignment
int execute_AST_assignment(struct AST *tree, char **argv);

// execute the ast operator (&& ||)
int execute_AST_operator(struct AST *tree, char **argv);

// do the PIPE AST execution
int execute_AST_pipe(struct AST *tree, char **argv);

// assign the function
int execute_AST_function(struct AST *tree);

// execute the given AST
// returns the status code after executing the AST
// returns 127 if a command is unknown
// suffixe traversal
int execute_AST_main(struct AST *tree, char **argv);

// execute the given AST
// returns the status code after executing the AST
// returns 127 if a command is unknown
// suffixe traversal
int execute_AST(struct AST *tree, char **argv);

#endif /* !EXECUTION_H */

