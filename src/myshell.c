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

//commands
void clear();
void dir(char ** args);
void cd (char ** args);
void getenviron();
void echo(char ** args);
void pauseshell();
int backgroundcheck(char ** args);
void external_command(char ** args);
void command_select(char **args);


int main(int argc, char*argv[])
{
    char * args[100];
    char ** arg;
    char buff[100];
    int batch = 1;
    
    char cwd[256];
    getcwd(cwd,sizeof(cwd));

    FILE *fp = stdin;
    if(argv[1]){
        batch = 0;
        fp = batchfile(argv[1]);
    }
    
    while(!feof(fp)){
        pid_t pid;

        if(batch == 1){
            prompt(cwd);
        }
        if(fgets(buff, sizeof(buff), (FILE*)fp))
        {

            arg = args;
            *arg++ = strtok(buff, SEPERATORS);
            while((*arg++ = strtok(NULL,SEPERATORS)));

            int waitint = backgroundcheck(args);
            
            switch(pid = fork())
            {
                case -1:
                    errorhandler("fork to run command selector.");
                    break;
                case 0: ; //silly little semicolon fixes declaring and assigning in a switch
                    int check = filecheck(args);
                    //puts("HELLO?");
                    if(check == 0){
                        command_select(args);
                    }
                    pid_t childpid = getpid();
                    kill(childpid, SIGTERM);
                    break;
                default:
                    if(waitint)
                    {
                        waitpid(pid, NULL, WUNTRACED);
                    }
                    else
                    {
                        waitpid(pid, NULL, WNOHANG);
                        sleep(1);
                    }
                    waitint = 1;
            }
            getcwd(cwd, sizeof(cwd));
        }
    }
    fclose(fp);
}

void prompt(char* cwd)
{
    printf("\033[0;36m");
    printf("%s >>> ",cwd);
    printf("\033[0m");
}

FILE* batchfile(char * file)
{
    FILE* fp;
    switch(access(file, F_OK))
    {
        case -1:
            printf("\033[0;31m");
            printf("Error: No such file %s exists\n", file);
            printf("\033[0m");
            exit(1);
        case 0:
            fp = fopen(file, "r");
    }
    return fp;
}

int backgroundcheck(char ** args)
{
    int position = 0;
    while(args[position + 1] != NULL)
    {
        position++;
    }
    if(strcmp(args[position], "&"))
    {
        return 1;
    }
    return 0;
}

void command_select(char ** args)
{
    if(strcmp(args[0],"quit") == 0){
        printf("QUIT COMMAND RECEIVED\n");
        exit(0);
    }
    if(strcmp(args[0], "clr") == 0){
        clear();
        return;
    }
    if(strcmp(args[0], "dir") == 0){
        dir(args);
        return;
    }
    if(strcmp(args[0], "cd") == 0){
        cd(args);
        return;
    }
    if(strcmp(args[0], "environ") == 0){
        getenviron();
        return;
    }
    if(strcmp(args[0], "echo") == 0){
        echo(args);
        return;
    }
    if(strcmp(args[0], "pause") == 0)
    {
        pauseshell();
        return;
    }
    else{
        external_command(args);   
        return;        
    }
}

int filecheck(char ** args)
{
    int i = 0;
    int input = -1;
    int output = -1;
    while(args[i] != NULL){
        if(strcmp(args[i], "<") == 0)
        {
            input = i + 1;
        }
        if((strcmp(args[i], ">") == 0) || (strcmp(args[i], ">>") == 0))
        {
            output = i + 1;
        }
        i++;
    }
    if(input != -1)
    {
        switch(access(args[input], F_OK)) // input file
        {
            case -1:
                printf("\033[0;31m");
                printf("Error: No such file %s exists\n", args[input]);
                printf("\033[0m");
                return 1;
            case 0:
                //printf("Input file is %s\n", args[input]);
                freopen(args[input], "r", stdin);
        }
    }
    if(output != -1)
    {
        if(strcmp(args[output - 1], ">") == 0)
        {
            printf("Opened %s for output truncating\n", args[output]);
            freopen(args[output], "w", stdout);
        }
        else if(strcmp(args[output - 1], ">>") == 0)
        {
            printf("Opened %s for output appending\n", args[output]);
            freopen(args[output], "a", stdout);
        }
    }
    return 0;
}

void clear()
{
    execlp("clear","clear",NULL);
    return;
}

void dir(char ** args)
{
    char base[] = "ls";
    char defaultarg[] = "-al";
    execlp(base,base,defaultarg,args[1],NULL);
    return;
}

void cd(char ** args)
{
    char cwd[256];
    getcwd(cwd,sizeof(cwd));
    if(!args[1])
    {
        printf("Current directory is: %s\n", cwd);
        return;
    }
    chdir(args[1]);
    getcwd(cwd,sizeof(cwd));
    char newpwd[] = "PWD=";
    strcat(newpwd, cwd);
    putenv(strdup(newpwd));
    return;
}

void getenviron()
{
    int count = 0;
    while(environ[count] != NULL)
    {
        puts(environ[count]);
        count++;
    }
    return;
}

void echo(char ** args)
{
    char string[100] = "";
    int i = 1;
    while(args[i] != NULL)
    {
        strcat(string, args[i]);
        strcat(string, " ");
        i++;
    }
    printf("\033[0;33m");
    printf("%s\n", string);
    printf("\033[0m");
    return;
}

void pauseshell()
{
    printf("\033[0;32m");
    printf("Press [Enter] key to continue.");
    printf("\033[0m");
    getchar();
    return;
}

void external_command(char ** args)
{
    int error;
    error = execvp(args[0],args);
    if(error == -1)
    {
            printf("\033[0;31m");
            printf("Unknown command, try another.\n");
            printf("\033[0m");
    }
    return;
}

void errorhandler(char * call)
{
    printf("\033[0;31m");
    printf("Error when executing %s\n", call);
    printf("\033[0m");
    return;
}