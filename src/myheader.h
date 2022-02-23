#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/wait.h>
#define SEPERATORS " \t\n"

extern char **environ;
extern int errno;     

void errorhandler(char*);
void prompt(char * cwd);
int filecheck(char ** args);
FILE* batchfile(char * file);
int lookforsymbol(char * arg);

//commands
void clear();
void dir(char ** args);
void cd (char ** args);
void getenviron();
void echo(char ** args);
void pauseshell();
int backgroundcheck(char ** args);
void external_command(char ** args);
void parentcommands(char ** args);
void command_select(char **args);