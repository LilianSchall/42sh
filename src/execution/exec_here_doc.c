#include "execution/execution.h"

static char *get_string(struct AST *tree)
{
    struct AST *ast_iter = tree->linked_list->head->next->next->data;

    char *string = ast_iter->value->values[0]->value;
    return string;
}

static void remove_tab(struct AST *tree)
{
    char *string = get_string(tree);

    int oldLen = strlen(string);
    int newLen = 0;

    while (oldLen != newLen)
    {
        oldLen = strlen(string);
        str_replace(string, "\n\t", "\n");
        newLen = strlen(string);
    }

    while (strlen(string) > 0 && string[0] == '\t')
    {
        for (size_t i = 0; i < strlen(string); i++)
        {
            string[i] = string[i + 1];
        }
    }
}

static int get_io_number(struct AST *tree)
{
    struct AST *ast = tree->linked_list->head->data;
    int io_number = my_itoa(ast->value->values[0]->value);
    return io_number;
}

static int execute_AST_pipe_v2(struct AST *tree, struct env *env)
{
    // création de la pipe
    int fd[2];

    int io_number = get_io_number(tree);

    int save2 = dup(io_number);
    int save = dup(STDOUT_FILENO);

    // on récupere le premier fils
    struct linked_node *child_list = tree->linked_list->head;

    // on execute le pipe !
    pipe(fd);

    // redirige stdout vers l'entrée de la pipe
    dup2(fd[1], STDOUT_FILENO);

    // execute la commande
    char *string = get_string(tree);
    printf("%s", string);
    if (string[strlen(string) - 1] != '\n')
        printf("\n");

    fflush(stdout);

    dup2(save, STDOUT_FILENO);
    // redirige stdin vers la sortie de la pipe

    dup2(fd[0], io_number);
    close(fd[0]);
    close(fd[1]);

    // execute la dernière commande (sera écrite dans stdout)
    int result = execute_AST_main(child_list->next->data, env);

    // on retablie les FD
    dup2(save2, io_number);
    dup2(save, STDOUT_FILENO);

    close(save2);
    close(save);

    return result;
}

int execute_AST_here_doc(struct AST *tree, struct env *env)
{
    if (tree->value->type == INF_INF_MIN)
    {
        remove_tab(tree);
    }

    return execute_AST_pipe_v2(tree, env);
}
