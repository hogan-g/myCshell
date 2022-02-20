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
void command_select(char **args);


int main(int argc, char*argv[])
{
    char * args[100]; // set up space to store arguments
    char ** arg; // pointer into args array
    char buff[100];
    int batch = 1; //signal variable default = no batch
    
    char cwd[256]; // variable to store cwd
    getcwd(cwd,sizeof(cwd)); // get starting cwd

    FILE *fp = stdin; //default file pointer for stdin
    if(argv[1]){ // if there is an argument after invocation of shell i.e. a batchfile
        batch = 0; //signal variable changes to say yes there is a batch file
        fp = batchfile(argv[1]); //invokes function while will overwrite stdin file pointer
    }
    
    while(!feof(fp)){ // while having not reached end of file of input file, stdin or batch
        pid_t pid;

        if(batch == 1){ // if there is no batch file print prompt
            prompt(cwd);
        }
        if(fgets(buff, sizeof(buff), (FILE*)fp)) // if able to get a line from user/batch
        {

            arg = args;
            *arg++ = strtok(buff, SEPERATORS); // split 1st argument from line by whitespace
            while((*arg++ = strtok(NULL,SEPERATORS))); // add all args to array, splitting as goes

            int waitint = backgroundcheck(args); // invokes funtion to check for & after program
            
            switch(pid = fork()) // main fork to send child off to do stuff
            {
                case -1: // case for if an error occurs when forking
                    errorhandler("fork to run command selector.");
                    break;
                case 0: ; // case for the child, here is what the child will do
                    int check = filecheck(args);
                    //puts("HELLO? (from puts)");
                    //printf("Hello? (From printf)\n");
                    //puts("WHat da frick?");
                    if(check == 0){
                        command_select(args);
                    }
                    //puts("After command? (from puts)");
                    //printf("after? (From printf)\n");
                    pid_t childpid = getpid(); //get pid of child
                    kill(childpid, SIGTERM); // kill the child after it is finished, prevents children spawning children
                    break;
                default: // what the parent process will do
                    if(waitint) // if the parent should wait
                    {
                        waitpid(pid, NULL, WUNTRACED);
                    }
                    else // if the parent shouldn't wait, i.e. for a background process
                    {
                        waitpid(pid, NULL, WNOHANG);
                        sleep(1);
                    }
                    waitint = 1;
            }
            getcwd(cwd, sizeof(cwd)); //refresh the stored cwd in case of updates to it (i.e. cd)
        }
    }
    fclose(fp); //closes the batchfile
}

void prompt(char* cwd) //function to print the prompt in cyan
{
    printf("\033[0;36m");
    printf("%s >>> ",cwd);
    printf("\033[0m");
}

FILE* batchfile(char * file) //batchfile checking function, returns update file pointer if there is one.
{
    FILE* fp;
    switch(access(file, F_OK)) // checks if file exists
    {
        case -1:
            printf("\033[0;31m");
            printf("Error: No such file %s exists\n", file);
            printf("\033[0m");
            exit(1);
        case 0:
            fp = fopen(file, "r"); // open batch file in reading mode
    }
    return fp; // return updated fp 
}

int backgroundcheck(char ** args) // function to check for &
{
    int position = 0;
    while(args[position + 1] != NULL) // checks index of last arg but iterating until end
    {
        position++;
    }
    if(strcmp(args[position], "&")) // if last argument is an &
    {
        return 1;  // return signal to say don't wait
    }
    return 0; // else keep signal yes for waiting
}

int filecheck(char ** args) // big function to check for input and output files within args
{
    int i = 0; // variable to track index as we iterate through args
    int input = -1; // default is no file for input or output, signalled by -1
    int output = -1;
    while(args[i] != NULL){ // iterate through args
        if(strcmp(args[i], "<") == 0) //check for input signal
        {
            input = i + 1; // if find < the file is the arg after that
        }
        if((strcmp(args[i], ">") == 0) || (strcmp(args[i], ">>") == 0)) // similar for output
        {
            output = i + 1;
        }
        i++; //increment i through args
    }
    if(input != -1) // if there was a file found
    {
        switch(access(args[input], F_OK)) // check access to input file
        {
            case -1: // error, no such file
                printf("\033[0;31m");
                printf("Error: No such file %s exists\n", args[input]);
                printf("\033[0m");
                return 1;
            case 0: // if file is fine
                //printf("Input file is %s\n", args[input]);
                freopen(args[input], "r", stdin); // open file replacing stdin
        }
    }
    if(output != -1) // if there was an output file found
    {
        if(strcmp(args[output - 1], ">") == 0) // truncating 
        {
            printf("Opened %s for output truncating\n", args[output]);
            freopen(args[output], "w", stdout); //open in w mode
        }
        else if(strcmp(args[output - 1], ">>") == 0) // appending
        {
            printf("Opened %s for output appending\n", args[output]);
            freopen(args[output], "a", stdout); // open in a mode
        }
    }
    return 0; // return signal saying all is fine
}

int lookforsymbol(char * arg)
{
    if(strcmp(arg,"<") == 0 || strcmp(arg, ">") == 0 || strcmp(arg, ">>") == 0)
    {
        return 0;
    }
    return 1;
}

void command_select(char ** args) // big switch case function to test if given command is internal
{
    if(strcmp(args[0],"quit") == 0){ // quit command
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
    else{ // if it matches none of our internal commands, send it to function for external commands
        external_command(args);   
        return;        
    }
}

void clear()
{
    execlp("clear","clear",NULL); // exec clear to clear screen
    return;
}

void dir(char ** args)
{
    char base[] = "ls"; // base command for dir is ls
    char defaultarg[] = "-al"; // default dir should always use -al
     // in case user wants to redirect default dir with no args, so ls won't try read < as a file
    if(args[1] != NULL && lookforsymbol(args[1]) == 0)
    {
        execlp(base,base,defaultarg,NULL);
    }
    else
    {
        execlp(base,base,defaultarg,args[1],NULL);
    }
    return;
}

void cd(char ** args) // cd command
{
    char cwd[256]; // holder for cwd
    getcwd(cwd,sizeof(cwd)); // get cwd
    if(!args[1]) // if there is no argument, print cwd
    {
        printf("Current directory is: %s\n", cwd);
        return;
    }
    chdir(args[1]); // change directory
    getcwd(cwd,sizeof(cwd)); // get new cwd
    char newpwd[] = "PWD="; // start of environ string
    strcat(newpwd, cwd); // append cwd to PWD= to create environ string
    putenv(strdup(newpwd)); // update environ string for PWD
    return;
}

void getenviron() // print our environ strings
{
    int count = 0;
    while(environ[count] != NULL) // iterate through external array containing env strings
    {
        puts(environ[count]); // print them out
        count++; //iterate
    }
    return;
}

void echo(char ** args) // internal echo command
{
    char string[100] = ""; // base string to add all args to
    int i = 1; // starting index for iterating thru args
    while(args[i] != NULL) //iterate
    {
        strcat(string, args[i]); //add args to string + a space inbetween
        strcat(string, " ");
        i++;
    }
    printf("\033[0;33m");
    printf("%s\n", string); //print the string in yellow
    printf("\033[0m");
    return;
}

void pauseshell() // pause shell
{
    printf("\033[0;32m");
    printf("Press [Enter] key to continue.");
    printf("\033[0m");
    getchar(); // waits until user presses enter
    return;
}

void external_command(char ** args) // for any unrecognised internal command, push to OS 
{
    int i = 0;
    char *argsmodified[100];
    // protection against commands seeing the I/O redirection symbols in their args, i.e. ls tries to read < as a dir
    while(lookforsymbol(args[i]) != 0) 
    {
        argsmodified[i] = args[i];
        i++;
    }

    int error;
    error = execvp(argsmodified[0],argsmodified); // exevp the whole array of args, returns a code
    if(error == -1) // if code signals an error, tell the user
    {
            printf("\033[0;31m");
            printf("Unknown command, try another.\n");
            printf("\033[0m");
    }
    return;
}

void errorhandler(char * call) // error handler function
{
    printf("\033[0;31m");
    printf("Error when executing %s\n", call);
    printf("\033[0m");
    return;
}