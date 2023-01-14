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
        fprintf(stderr, "42sh: %s: cannot overwrite existing file\n",
            filename);
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
        return my_itoa(tree->value->symbol);

    char *filename = NULL;

    // check if filename is a string to expand
    if (tree->value->is_expandable)
        filename = expand_var(tree->value->symbol);
    else
        filename = tree->value->symbol; // get the filename

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
        int val= my_itoa(filename);
        // check if it as FD to close 
        if(!strcmp(filename, "-"))
            ret_val = -2;

        // check if the filename is a number -> IO_NUMBER
        else if (val != -1)
            ret_val = val;
        else 
        {
            if (r_type == R_SUP_AND) // >&
                ret_val = open(filename, O_CREAT | O_TRUNC | O_WRONLY | O_CLOEXEC, 0755);
            else if (r_type == R_INF_AND) // <&
                ret_val = check_if_file_exist(filename);
        }
    }
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
