#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "garbage_collector/garbage_collector.h"

char *expand_var(const char *str);

#endif /* !VARIABLES_H */

