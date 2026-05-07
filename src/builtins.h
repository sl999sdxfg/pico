#ifndef BUILTINS_H
#define BUILTINS_H

int exit_(char *argv[]);
int cd(char *argv[], char *cwd, size_t cwd_size);
int pwd();
int echo(char *argv[]);
int export(char *argv[]);

#endif