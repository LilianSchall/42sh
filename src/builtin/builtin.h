#ifndef BUILTIN_H
#define BUILTIN_H

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "AST/AST.h"
#include "commons/commons.h"
#include "execution/execution.h"
#include "expansion/expansion.h"
#include "garbage_collector/garbage_collector.h"
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

// set the *val to argv[1] if it exist, else set to 1
int exec_break_continue(int argc, char **argv, int *val);

// do the exit function and the the status structure in order to exit
int exit_fn(int argc, char **argv);

// echo  builtin function
// take argc & argv in parameter
int echo_fn(int argc, char **argv);

// return 0
int true_fn(int argc, char **argv);

// return 1
int false_fn(int argc, char **argv);

int cd_fun(int argc, char **argv);

int exec_export(int argc, char *argv[], char **envp);
#endif /* !BUILTIN_H */

