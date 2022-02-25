// Made by Gareth Hogan
// Student Num: 20379616
// I have read and acknowledge the DCU Academic Integrity and Plagiarism Policy. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/wait.h>
#define SEPERATORS " \t\n"

extern char **environ;
extern int errno;     

// utility/shell functions
void errorhandler(char*);
void prompt(char * cwd);
int filecheck(char ** args);
FILE* batchfile(char * file);
int lookforsymbol(char * arg);
int backgroundcheck(char ** args);
void external_command(char ** args);
void parentcommands(char ** args);
void command_select(char **args);
void setenvstrings(char ** argv);

// commands
void clear();               //clr
void dir(char ** args);     //dir
void cd (char ** args);     //cd
void getenviron();          //environ
void echo(char ** args);    //echo
void pauseshell();          //pause
void showhelp();            //help