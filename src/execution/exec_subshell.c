#include "execution/execution.h"

char *get_content_of_pipe(int pipefd[2])
{
    close(pipefd[1]);
    char buffer[1024];
    char *output = malloc(sizeof(char));
    size_t size = 0;
    size_t i = 0;

    size_t nb_read = read(pipefd[0], buffer, sizeof(buffer));

    while (nb_read > 0)
    {
        size += nb_read;
        output = realloc(output, sizeof(char) * (size + 1));

        for (size_t j = 0; j < nb_read; j++)
        {
            output[i] = buffer[j];
            i++;
        }
        output[i] = '\0';
        nb_read = read(pipefd[0], buffer, sizeof(buffer));
    }
    close(pipefd[0]);
    return output;
}

char *execute_AST_D_SUBSHELL(struct AST *tree, struct env *env)
{
    int pipefd[2];
    pipe(pipefd);

    int pid = fork();
    if (pid == 0)
    {
        // child process
        close(pipefd[0]);

        int save1 = dup(STDOUT_FILENO);

        dup2(pipefd[1], STDOUT_FILENO);
        struct AST *child = tree->linked_list->head->data;
        execute_AST_main(child, env);
        fflush(stdout);

        close(pipefd[1]);
        dup2(save1, STDOUT_FILENO);
        close(save1);
        exit(0);
    }
    else
    {
        // parent process
        char *result = get_content_of_pipe(pipefd);
        wait(NULL);
        return result;
    }
}

int execute_AST_subshell(struct AST *tree, struct env *env)
{
    int ret_val = 0;
    int pid = fork();

    if (!pid) // child goes in
    {
        struct AST *child = tree->linked_list->head->data;
        ret_val = execute_AST_main(child, env);
        exit(ret_val);
    }
    wait(&ret_val);
    if (WIFEXITED(ret_val))
        return WEXITSTATUS(ret_val);
    return 0;
}
