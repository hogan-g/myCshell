# CA216 Operating Systems: myshell Assignment Gareth Hogan

## myshell man

## START UP COMMAND
    ./bin/myshell [file path (optional for batch commands)]

## INTRODUCTION
    Welcome to myshell, this program is a small example of a command line interpreter shell environment where commands and programs 
    can be run from standard input (user) or from a file (batch file).
    Once invoked myshell provides a shell environment which can be used to run basic UNIX commands and also user created programs, 
    the shell will provide the user with a prompt, beside which you can write commands and programs to be executed by the program. [4]
    myshell is a program built using the C language and uses functions and structures from c. [2] 

## OPTIONS
    .bin/myshell [file]
    myshell only has one option when being invoked, it is an optional space for a path to a file. If this file contains commands 
    and programs that can be interpreted and run by the shell it acts as a replacement for taking input from the user. 
    Shell Scripting or Batch Programming as it is known can be used to run lots of commands in succession without the need for 
    the user to type them all out.

    batchfile operation is achieved by switching the default input stream of the program. In normal use the input stream is stdin which is
    the users terminal, when a file argument is given this stream is instead switched to the file path given, and lines of input are read
    from the file until the EOF (end of file) is reached.

## INVOCATION
    ./bin/myshell
    After being invoked by the user without a batch file, the program provides an interactive program environment for the user 
    to run commands and programs, by default the shell will then output the results of these commands to the terminal under 
    the prompt where the user entered the command.

##  COMMANDS
    help - will show this manual in the terminal
    clr - will clear the screen of all text and give a new prompt for the user to use
    dir [directory] - will list the contents of the directory given (defaults to current directory if no argument given)
    cd [path] - will change the current working directory to the given path (will print CWD if no argument given)
    environ - will print out all environment strings for user to see
    echo [text] - will output the given text back to standard output
    pause - will pause operation of the shell until the user presses the [ENTER] key
    quit- will close the shell program

    EXTERNAL COMMANDS
    All other commands will be pushed back out to the operating system and executed using the C system() function.
    This should support every command normally possibly executed on your OS. For example all general linux commands should 
    work if you ran myshell using linux. [3]

    Processing commands is achieved by using functions built in the myshell program, after a line of input is read from the input
    stream, it is split into what it contains and then a child process is born to do everything to do with that line form now on, the program
    forks and sends the child to check exactly what command has been input and what options are present (e.g. background processing or I/O 
    redirection). This child process then executes whatever functions are necessary until an output is pushed to the output stream, and the
    child returns and is killed by the parent.

## CONTROL SYMBOLS
    & - Flag as Background Process
    < - signals next argument is an input file
    > - signals next argument is a filename to output to (creates the file if it does not exist, overwrites it if it does)
    >> - signals next argument is a filename to output to (creates the file if it does not exists, appends to it if it does)

## I/O REDIRECTION [5]
    If the user wishes to change where the output of commands and programs is printed to or where the input for a program or 
    command is fetched from they can do so using I/O redirection and it's corresponding control symbols (<, >, >>)
    INPUT REDIRECTION
    If the user inputs a < symbol followed by a valid filename after their command, the shell will look in that file for 
    input data instead of asking the user for it in the command or asking them to input it on following lines. 
    OUTPUT REDIRECTION
    If the user inputs either of the output symbols (> or >>) followed by a filename, the shell will switch to print all 
    output for that command into the file named, creating it if it does not already exist.

    I/O Redirection is achieved by changing the input and output streams from their standard settings. By default input and output link to the 
    users terminal and keyboard. If the control symbols for I/O redirection are detected in a command by the child process the process enters 
    a special function which replaces the input and output streams with files as necessary using freopen(). Then the command executes using these
    new streams, after execution is complete the child is killed, the parent process still has the normal I/O streams open, and the shell moves
    on to the next command given.

## BACKGROUND PROGRAM EXECUTION
    If the user inputs an & as the last argument after a command, the shell will execute this command in the background.
    This means that if the program/command takes a while to complete, the user will be given another prompt and may do other 
    commands while the background program is running, it will print output to the user as soon as it is finished.

    Background processing is achieved by flagging to the parent process whether or not to wait for a child to finish. By default the parent
    process will wait after sending a child off to execute a command. It will wait until the child is completely finished before moving on to
    try read in another command from the user. If the & control symbol is present after a command, a flag is triggered telling the parent
    not to wait for the child executing that command. Therefore the parent can continue to take in commands and make other children, while the 
    background child works on it's program, giving the user output when it is finished.

### REFERENCES
    Below are references to reading material that helped me for the whole project, they are referenced in the above manual where they 
    are related to the topic, [1] the bash man page was the main source of info for this man page.

    [1] “Linux bash man page,” Bash(1) - linux manual page. [Online]. Available: https://www.man7.org/linux/man-pages/man1/bash.1.html. [Accessed: 26-Feb-2022]. 
    [2] “C Documentation,” DevDocs. [Online]. Available: https://devdocs.io/c/. [Accessed: 26-Feb-2022]. 
    [3] A. Le Morvan, S. Spencer, and A. Putta, “Commands for linux users,” Linux Commands - Documentation, 05-Jan-2022. [Online]. Available: https://docs.rockylinux.org/books/admin_guide/03-commands/. [Accessed: 26-Feb-2022]. 
    [4] D. B. Both, “How to program with bash: Syntax and Tools,” Opensource.com, 21-Oct-2019. [Online]. Available: https://opensource.com/article/19/10/programming-bash-syntax-tools. [Accessed: 26-Feb-2022]. 
    [5] D. Collazo, “An introduction to linux I/O redirection,” DigitalOcean, 23-Jan-2014. [Online]. Available: https://www.digitalocean.com/community/tutorials/an-introduction-to-linux-i-o-redirection. [Accessed: 26-Feb-2022]. 

## Made by Gareth Hogan
## Student Num: 20379616
## I have read and acknowledge the DCU Academic Integrity and Plagiarism Policy. 