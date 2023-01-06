#include "execution.h"


// execute the node only when the parent node is a COMMAND type
// will do a 
int executee_node_AST(struct AST *tree)
{
    (void) tree;
    return 0;
}

int execute_AST(struct AST *tree)
{
    return executee_node_AST(tree);
}
