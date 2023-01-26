#ifndef EXECUTION_H
#define EXECUTION_H

#include <err.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "AST/AST.h"
#include "builtin/builtin.h"
#include "functions/functions.h"
#include "garbage_collector/garbage_collector.h"
#include "linked_list/linked_list.h"

// execute ast d_subshell
// return the string printed in stdout of the subshell
char *execute_AST_D_SUBSHELL(struct AST *tree, struct env *env);

// execute AST case
int execute_AST_case(struct AST *tree, struct env *env);

//execute here-document AST
int execute_AST_here_doc(struct AST *tree, struct env *env);

// execute D_SUBSHELL ast $(..)
char *execute_AST_D_SUBSHELL(struct AST *tree, struct env *env);

// execute SUBSHELL ast (..)
int execute_AST_subshell(struct AST *tree, struct env *env);

// execute ast redirection ( > >> < >& <> >| <& )
int execute_AST_redirection(struct AST *tree, struct env *env);

// execute the ast condition (for while until if)
int execute_AST_condition(struct AST *tree, struct env *env);

// execute the ast assignment
int execute_AST_assignment(struct AST *tree, struct env *env);

// execute the ast operator (&& ||)
int execute_AST_operator(struct AST *tree, struct env *env);

// do the PIPE AST execution
int execute_AST_pipe(struct AST *tree, struct env *env);

// assign the function
int execute_AST_function(struct AST *tree, struct env *env);

// execute the given AST
// returns the status code after executing the AST
// returns 127 if a command is unknown
// suffixe traversal
int execute_AST_main(struct AST *tree, struct env *env);

// execute the given AST
// returns the status code after executing the AST
// returns 127 if a command is unknown
// suffixe traversal
int execute_AST(struct AST *tree, char **argv, struct linked_list *functions);

#endif /* !EXECUTION_H */
