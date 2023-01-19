#include "execution/execution.h"

// redirect fd_from file descriptor into into fd_to file descriptor
int redirection_fd_to_fd(struct AST *tree, int fd_from, int fd_to)
{
    // duplicate fd_from file descriptor

    int from_dup = dup(fd_from);

    if (fd_to != -2)
    {
        // put file_fd into fd_to
        if (dup2(fd_to, fd_from) == -1)
        {
            close(from_dup);
            return 2;
        }
    }
    // we are in case : >&-
    // we need to close fd_from
    else
    {
        close(fd_from);
    }

    // excute the SEQUENCE or REDIRECTION  AST
    int return_val = execute_AST(tree);

    // restore fd
    dup2(from_dup, fd_from);
    // close all file descriptor
    close(from_dup);

    return return_val;
}

// check if file exist
// if yes, it opens it in read and return FD
// else, return -1 and print error message
int check_if_file_exist(char *filename)
{
    int ret_val;
    if (!access(filename, F_OK)) // check if file exist
        ret_val = open(filename, O_RDONLY | O_CLOEXEC);
    else
    {
        fprintf(stderr, "42sh: %s: cannot overwrite existing file\n", filename);
        ret_val = -1;
    }
    return ret_val;
}

// return the file descriptor
// return the FD if tree as a token IO_NUMBER
// return the FD of the file opened with the good option depend on the r_type
int get_fd_from_ast(struct AST *tree, enum token_type r_type)
{
    if (tree->value->type == IO_NUMBER) // IO_NUMBER (0,1,2,...)
        return my_itoa(tree->value->values[0]->value);

    char *filename = NULL;

    // check if filename is a string to expand
    if (tree->value->values[0]->is_expandable)
        filename = expand_var(tree->value->values[0]->value, 0);
    else
        filename = tree->value->values[0]->value; // get the filename

    int ret_val = 0;

    if (r_type == R_SUP_SUP) // >>
        ret_val =
            open(filename, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0755);
    else if (r_type == R_INF_SUP) // <>
        ret_val = open(filename, O_CREAT | O_RDWR | O_CLOEXEC, 0755);
    else if (r_type == R_SUP_PIPE || r_type == R_SUP) //  >|   >
        ret_val =
            open(filename, O_CREAT | O_TRUNC | O_WRONLY | O_CLOEXEC, 0755);
    else if (r_type == R_INF_SUP) // <>
        ret_val = open(filename, O_CREAT | O_RDWR | O_CLOEXEC, 0755);
    else if (r_type == R_INF) // <
        ret_val = check_if_file_exist(filename);
    else
    {
        int val = my_itoa(filename);
        // check if it as FD to close
        if (!strcmp(filename, "-"))
            ret_val = -2;

        // check if the filename is a number -> IO_NUMBER
        else if (val != -1)
            ret_val = val;
        else
        {
            if (r_type == R_SUP_AND) // >&
                ret_val = open(filename,
                               O_CREAT | O_TRUNC | O_WRONLY | O_CLOEXEC, 0755);
            else if (r_type == R_INF_AND) // <&
                ret_val = check_if_file_exist(filename);
        }
    }
    if (tree->value->values[0]->is_expandable)
    {
        mem_free(filename);
    }
    return ret_val;
}

// close the file descriptor if tree has NOT a token IO_NUMBER
void close_fd(int fd, struct AST *tree)
{
    if (tree->value->type != IO_NUMBER)
        close(fd);
}



int execute_AST_redirection(struct AST *tree)
{
    int return_val = 0;
    enum token_type r_type = tree->value->type;

    struct linked_node *child_list = tree->linked_list->head;

    struct AST *ast_from = child_list->data;
    struct AST *ast_exec = child_list->next->data;
    struct AST *ast_to = child_list->next->next->data;

    int fd_from = get_fd_from_ast(ast_from, r_type);
    if (fd_from == -1)
        return 2;

    int fd_to = get_fd_from_ast(ast_to, r_type);
    if (fd_to == -1)
    {
        close_fd(fd_from, ast_from);
        return 2;
    }

    return_val = redirection_fd_to_fd(ast_exec, fd_from, fd_to);

    close_fd(fd_from, ast_from);
    close_fd(fd_to, ast_to);

    return return_val;
}