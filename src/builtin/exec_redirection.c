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
    int return_val = execute_AST(tree);

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

// redirect fd_from file descriptor into into fd_to file descriptor
int redirection_fd_to_fd(struct AST *tree, int fd_from, int fd_to)
{
    // duplicate fd_from file descriptor
    int from_dup = dup(fd_from);

    // put file_fd into fd_to
    if (dup2(fd_to, fd_from) == -1)
    {
        close(from_dup);
        return 2;
    }
    // excute the SEQUENCE or REDIRECTION  AST
    int return_val = execute_AST(tree);
    // restore fd
    dup2(from_dup, fd_from);
    // close all file descriptor
    close(from_dup);

    return return_val;
}

// return the file descriptor
// return the FD if tree as a token IO_NUMBER
// return the FD of the file opened with the good option depend on the r_type
int get_fd_from_ast(struct AST *tree, enum token_type r_type)
{
    if (tree->value->type == IO_NUMBER) // IO_NUMBER (0,1,2,...)
        return tree->value->symbol[0] - '0';

    char *filename = NULL;
    if (tree->value->is_expandable)
    {
        filename = expand_var(tree->value->symbol);
    }
    else
    {
        filename = tree->value->symbol; // get the filename
    }

    int ret_val = 0;

    if (r_type == R_SUP_SUP) // >>
        ret_val = open(filename, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0755);
        
    else if (r_type == R_SUP_PIPE || r_type == R_SUP) //  >|   >
        ret_val = open(filename, O_CREAT | O_TRUNC | O_WRONLY | O_CLOEXEC, 0755);

    else if (r_type == R_SUP_AND) // >&
        ret_val = open(filename, O_CREAT | O_TRUNC | O_WRONLY | O_CLOEXEC, 0755);

    else if (r_type == R_INF || r_type == R_INF_AND) // < or <&
    {
        if (!access(filename, F_OK)) // check if file exist
            ret_val = open(filename, O_RDONLY | O_CLOEXEC);
        else
        {
            fprintf(stderr, "42sh: %s: cannot overwrite existing file\n",
                tree->value->symbol);
            ret_val = -1;
        }
    }

    else if (r_type == R_INF_SUP) // <>
        ret_val = open(filename, O_CREAT | O_RDWR | O_CLOEXEC, 0755);

    if (tree->value->is_expandable)
    {
        free(filename);
    }
    return ret_val;
}

// close the file descriptor if tree has NOT a token IO_NUMBER
void close_fd(int fd, struct AST *tree)
{
    if (tree->value->type != IO_NUMBER)
        close(fd);
}
