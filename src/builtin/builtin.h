#ifndef BUILTIN_H
#define BUILTIN_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>


#include "AST/AST.h"
#include "commons/commons.h"
#include "execution/execution.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

// struct to stock echo option
struct echo_option
{
    char option_n; // -n inhibits printing a newline
    char option_E; // -e enable the interpretation of \n, \t and \\ escapes
    char option_e; // -E disable the interpretation of \n, \t and \\ escapes.
};

// echo  builtin function
// take argc & argv in parameter
int echo_fn(int argc, char **argv);

// return 0
int true_fn(int argc, char **argv);

// return 1
int false_fn(int argc, char **argv);

// do the < redirection
// first do the stderr redirection then the stdout redirection
int exec_inf_redirection(struct AST *tree);


// do the >& AST redirection
int redirection_stderr_stdout(struct AST *tree, char *filename);

// do the > AST redirection if bool_edit = 0
// do the >> AST redirection if bool_edit = 1
int redirection_stdout(struct AST *tree, char *filename, int bool_edit);

// do the > AST redirection if bool_edit = 0
// do the >> AST redirection if bool_edit = 1
int redirection_stderr(struct AST *tree, char *filename, int bool_edit);

// do the > AST redirection if bool_edit = 0
// do the >> AST redirection if bool_edit = 1
int redirection_stdin(struct AST *tree, char *filename, int bool_edit);

// redirect fd_from file descriptor into into fd_to file descriptor
int redirection_fd_to_fd(struct AST *tree, int fd_from, int fd_to);

#endif /* !BUILTIN_H */
