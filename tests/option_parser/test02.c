#include <criterion.h>

#include "option_parser/option_parser.h"

// test to test basic echo function

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