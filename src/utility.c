// Made by Gareth Hogan
// Student Num: 20379616
// I have read and acknowledge the DCU Academic Integrity and Plagiarism Policy. 

void prompt(char* cwd) //function to print the prompt in cyan
{
    printf("\033[0;36m");
    printf("%s >>> ",cwd);
    printf("\033[0m");
}

void parentcommands(char ** args)
{
    if(strcmp(args[0],"quit") == 0){ // quit command
        exit(0);
    }
    if(strcmp(args[0], "cd") == 0){
        cd(args);
        return;
    }
}

void command_select(char ** args) // big switch case function to test if given command is internal
{
    if(strcmp(args[0], "clr") == 0){
        clear();
        return;
    }
    if(strcmp(args[0], "dir") == 0){
        dir(args);
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
    if(strcmp(args[0], "help") == 0)
    {
        showhelp();
        return;
    }
    if (strcmp(args[0],"quit") == 0 || strcmp(args[0],"cd") == 0)
    {
        return;
    }
    else{ // if it matches none of our internal commands, send it to function for external commands
        external_command(args);   
        return;        
    }
}

int lookforsymbol(char * arg)
{
    if(strcmp(arg,"<") == 0 || strcmp(arg, ">") == 0 || strcmp(arg, ">>") == 0)
    {
        return 0;
    }
    return 1;
}

void errorhandler(char * call) // error handler function
{
    printf("\033[0;31m");
    printf("Error when executing %s\n", call);
    printf("\033[0m");
    return;
}