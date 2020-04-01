#ifndef  _SMSH_H
#define  _SMSH_H

char *next_cmd();
char **splitline(char *);
char *newstr();
void freelist();
void *emalloc(size_t);
void *erealloc(void *, size_t);
int execute(char **);
void fatal(char *, char *, int);


int process(char **);

int ok_to_execute();
int is_control_command(char *s);
int do_control_command(char **args);

int builtin_command();
#endif
