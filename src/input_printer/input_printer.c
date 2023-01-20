#include "input_printer.h"

static void replace_chars(char *command, const char *input,
        const char *newline, const char space_replace)
{
    size_t j = 0;
    for (size_t i = 0; input[i]; i++)
    {
        if (input[i] == ' ')
            command[j++] = space_replace;
        else if (input[i] != '\n')
            command[j++] = input[i];
        else
        {
            for (size_t k = 0; newline[k]; k++)
                command[j++] = newline[k];            
        }
    }
}

static size_t my_strcount(char *str, char c)
{
    size_t count = 0;
    
    for (size_t i = 0; str[i]; i++)
    {
        if (str[i] == c)
            count++;
    }

    return count;
}

void pretty_print_input(char *input)
{
    size_t len = strlen(input);

    size_t nb_newline = my_strcount(input, '\n');
    const char *newline = "NEWLINE";
    const size_t len_newline = 7;

    char *command = mem_calloc(len + nb_newline * len_newline + 1, 1);

    replace_chars(command, input, newline, '%');

    puts("BEGIN COMMAND");
    
    printf("%s\n", command);

    puts("END COMMAND");

    mem_free(command);
}
