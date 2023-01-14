#include "builtin/builtin.h"

int execute_AST_pipe(struct AST *tree)
{
    // création de la pipe
    int fd[2];

    int pid;

    int save2 = dup(STDIN_FILENO);

    // on récupere le premier fils
    struct linked_node *child_list = tree->linked_list->head;
    struct AST *child = child_list->data;
    
    // on boucle sur tous les fils sauf le dernier !
    while (child_list->next != NULL)
    {

        pipe(fd);
        if ((pid = fork()) == 0) 
        {
            // redirige stdout vers l'entrée de la pipe
            int save = dup(STDOUT_FILENO);
            dup2(fd[1], STDOUT_FILENO); 
            close(fd[0]);
            close(fd[1]);
            execute_AST(child);  // execute la commande
            dup2(save, STDOUT_FILENO); 
            exit(0);
        } 
        else
        {

            wait(NULL);
            // redirige stdin vers la sortie de la pipe
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
        }
        

        // on récupère l'enfant suivant
        child_list = child_list->next;
        child = child_list->data;
    }
    
    // execute la dernière commande (sera écrite dans stdout)
    int result = execute_AST(child);

            dup2(save2, STDIN_FILENO);
    close(fd[0]);
    close(fd[1]);
    return result;
}
