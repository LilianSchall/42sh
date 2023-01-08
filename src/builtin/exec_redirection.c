#include "builtin/builtin.h"

int redirection_stdout(struct AST *tree, char *filename, int bool_edit)
{
    int file_fd;
    // open our file
    if (bool_edit)
        file_fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0755);
    else
        file_fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0755);

    if (file_fd == -1)
        return 2;
    
    // duplicate STDOUT file descriptor
    int stdout_dup = dup(STDOUT_FILENO);

    // put file_fd into stdout
    if (dup2(file_fd, STDOUT_FILENO) == -1)
    {
        close(file_fd);
        return 2;
    }

    // do stuff
    int return_val = 0;
    if (tree->type == COMMAND)
            execute_AST_cmd(tree);
    if (tree->type == REDIRECTION)
            execute_AST_redirection(tree);

    fflush(stdout);
    // end stuff

    // restore stdout
    dup2(stdout_dup, STDOUT_FILENO);

    // close all file descriptor
    close(file_fd);
    close(stdout_dup);

    return return_val;
}


int exec_sup_redirection(struct AST *tree, int append)
{
    int return_val = 0;
    char *filename;

    struct linked_node *node = tree->linked_list->head;
    struct AST *child = node->data; // command
    struct AST *child2 = node->next->data; // redirection file

    if (child2->type == ARG)
    {
        filename = child2->value->symbol;
        return_val = redirection_stdout(child, filename, append);
    }

    return return_val;
}