#ifndef OPTION_PARSER_H
#define OPTION_PARSER_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "kernel/launch_options.h"

// this function parse the given argument and returns an integer
// representing every option activated bitwisely (defined in launch_options.h).
// if a filename for a script is given, store the filename into filename
// else set it to NULL
// if an error occurred, returns -1
int parse_given_args(int argc, char **argv, char **filename, char **input);

// this function checks bitwisely if a current mode is activated in options
bool is_option_activated(int options, int mode);

#endif /* !OPTION_PARSER_H */
