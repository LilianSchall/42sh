#include "builtin/builtin.h"

// redirect stdout & stderr into file named filename
int redirection_stderr_stdout(struct AST *tree, char *filename)
{
    int file_fd;
    // open our file
    file_fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0755);

    if (file_fd == -1)
        return 2;
    
    // duplicate STDOUT file descriptor
    int stderr_dup = dup(STDERR_FILENO);
    int stdout_dup = dup(STDOUT_FILENO);

    // put file_fd into stderr
    if (dup2(file_fd, STDERR_FILENO) == -1)
    {
        close(file_fd);
        return 2;
    }
    // put file_fd into stdout
    if (dup2(file_fd, STDOUT_FILENO) == -1)
    {
        close(file_fd);
        return 2;
    }
    // do stuff
    int return_val = 0;
    if (tree->type == COMMAND)
            return_val = execute_AST_cmd(tree);
    if (tree->type == REDIRECTION)
            return_val = execute_AST_redirection(tree);

    fflush(stderr);
    fflush(stdout);
    // end stuff

    // restore stdout
    dup2(stderr_dup, STDERR_FILENO);
    dup2(stdout_dup, STDOUT_FILENO);

    // close all file descriptor
    close(file_fd);
    close(stderr_dup);
    close(stdout_dup);

    return return_val;
}

// redirect stdout into file named filename
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
            return_val = execute_AST_cmd(tree);
    if (tree->type == REDIRECTION)
            return_val = execute_AST_redirection(tree);

    fflush(stdout);
    // end stuff

    // restore stdout
    dup2(stdout_dup, STDOUT_FILENO);

    // close all file descriptor
    close(file_fd);
    close(stdout_dup);

    return return_val;
}

// add the AST tree 'arg' into the child list of the first COMMAND AST found
void add_arg_command_ast(struct AST *tree, struct AST *arg)
{
    if(tree->type == COMMAND)
    {
        tree->linked_list = list_append(tree->linked_list, arg);
        return;
    }
    
    add_arg_command_ast(tree->linked_list->head->data, arg);
}

// do the < redirection
int exec_inf_redirection(struct AST *tree)
{
    int return_val = 0;

    struct linked_node *node = tree->linked_list->head;
    struct AST *child = node->data; // command
    struct AST *child2 = node->next->data; // redirection file

    // move AST child2 into link_list of AST child
    add_arg_command_ast(child, child2);

    // remove it from the 'tree' children
    node->next->data = NULL;

    if (child->type == COMMAND)
            return_val = execute_AST_cmd(child);
    if (child->type == REDIRECTION)
            return_val = execute_AST_redirection(child);

    return return_val;
}