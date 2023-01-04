#ifndef IO_BACKEND_H
#define IO_BACKEND_H

// function that returns the content of a filename
char *get_file_content(char *filename);

// function that gets the content of the input stored in stdin
char *get_interactive_content(void);

#endif /* !IO_BACKEND_H */
