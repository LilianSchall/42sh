#ifndef KERNEL_H
#define KERNEL_H

// launches the shell with the defined options in launch_options.h
// if launched in interactive mode, file_script = NULL
// returns 1 if an error occurred, else 0
int launch_shell(int options, char *file_script);


// function that will call each individual functions from the other modules
// in order to execute a shell command
// returns 1 if an error occurred, else 0
int execute_shell_commands(int options, char *input);

#endif /* !KERNEL_H */
