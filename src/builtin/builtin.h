#ifndef BUILTIN_H
#define BUILTIN_H

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "AST/AST.h"
#include "commons/commons.h"
#include "execution/execution.h"
#include "io_backend/io_backend.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

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

// return the file descriptor
// return the FD if tree as a token IO_NUMBER
// return the FD of the file opened with the good option
int get_fd_from_ast(struct AST *tree, enum token_type r_type);

// close the file descriptor if tree has NOT a token IO_NUMBER
void close_fd(int fd, struct AST *tree);

// do the >& AST redirection
int redirection_stderr_stdout(struct AST *tree, char *filename);

// redirect fd_from file descriptor into into fd_to file descriptor
int redirection_fd_to_fd(struct AST *tree, int fd_from, int fd_to);


// do the PIPE AST execution
int execute_AST_pipe(struct AST *tree);

#endif /* !BUILTIN_H */
