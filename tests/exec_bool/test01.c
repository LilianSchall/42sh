#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "symbol/symbol.h"
#include "token/token.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>



extern struct AST * new_AST_COMMAND(char *command, char *val1, char *val2);

int exec_bool(void) 
{

  struct AST *ast = new_AST_COMMAND("true", NULL, NULL);

  int result = execute_AST(ast, NULL);

  free_AST(ast);

  return result;
}

Test(exec_bool, basic_true) { cr_assert_eq(exec_bool(), 0); }
