#ifndef KERNEL_H
#define KERNEL_H

#include <unistd.h>

#include "execution/execution.h"
#include "garbage_collector/garbage_collector.h"
#include "io_backend/io_backend.h"
#include "launch_options.h"
#include "lexer/lexer.h"
#include "linked_list/linked_list.h"
#include "option_parser/option_parser.h"
#include "parser/parser.h"

// launches the shell with the defined options in launch_options.h
// if launched in interactive mode, file_script = NULL
// returns 1 if an error occurred, else 0
int launch_shell(int options, char *file_script, char *input);

// function that will call each individual functions from the other modules
// in order to execute a shell command
// returns the status code of the execution of the command
int execute_shell_command(int options, char *input);

#endif /* !KERNEL_H */

