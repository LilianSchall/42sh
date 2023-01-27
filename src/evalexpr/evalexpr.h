#ifndef EVALEXPR_H
#define EVALEXPR_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "evalexpr_ast.h"
#include "garbage_collector/garbage_collector.h"

char *evalexpr(char *str);

#endif /* !EVALEXPR_H */
