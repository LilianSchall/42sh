#include <criterion/criterion.h>

#include "AST/AST.h"
#include "exec_echo/exec_echo.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

/* 

AST = 
{
  enum AST_type type = COMMAND;
  struct token *value = { WORD, 'echo'};
  struct AST *left_child = { 
      ARG, 
      { WORD, '-e' } 
      {
        ARG, 
        {WORD, 'test'},
         NULL,
         NULL
      },
      NULL
  };
  struct AST *right_child = NULL;
};

*/


// struct token *new_token(char *symbol, enum token_type type);
// struct linked_list *new_list(void);
// struct linked_list *list_append(struct linked_list *list, void *data);

// struct AST *new_AST(struct token *value, enum AST_type type, struct linked_list *linked_list);


int main(void)
{
    struct AST *ast_word = (new_token('foooo', token_type.WORD), AST_type.ARG, NULL);

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_word);
    
    struct AST *ast = (new_token('echo', token_type.WORD), AST_type.COMMAND, ll_ast);



    return 0;
}