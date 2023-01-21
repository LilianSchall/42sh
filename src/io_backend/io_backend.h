#ifndef IO_BACKEND_H
#define IO_BACKEND_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "garbage_collector/garbage_collector.h"

// function that returns the content of a filename
char *get_file_content(char *filename);

// function that gets the content of the input stored in stdin
char *get_interactive_content(bool read_mode);

#endif /* !IO_BACKEND_H */
