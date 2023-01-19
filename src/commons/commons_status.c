#include "commons/commons.h"

struct status *status = NULL;

void new_status(void)
{
    if(status != NULL)
        return;
    status = mem_malloc(sizeof(struct status));

    status->continue_val = 0;
    status->break_val = 0;
    status->nb_loop = 0;
    status->exit_code = 0;
    status->exit_bool = 0;
}

void free_status(void)
{
    if(status == NULL)
        return;

    mem_free(status);
}

// return true if we need to continue in boucle
// return false if we need to stop
int check_status(void)
{
    if (status == NULL)
        return 1;

    return (status->continue_val == 0 && status->break_val == 0 && 
        status->exit_bool == 0);
}

void increase_nb_loop(void)
{
    if(status)  
        status->nb_loop += 1;
}

void decrease_nb_loop(void)
{
    if(status)  
        status->nb_loop -= 1;
}