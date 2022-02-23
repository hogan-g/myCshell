# CA216 Operating Systems: myshell Assignment Gareth Hogan

## START UP COMMAND
    ./bin/myshell [file(optional for batch commands)]

## INTRODUCTION
    Welcome to myshell, this program is a small example of a command line interpreter shell environment where commands and programs can be run from standard input (user) or from a file (batch file).
    Once invoked myshell provides a shell environment which can be used to run basic UNIX commands and also user created programs, the shell will provide the user with a prompt, beside which you can write commands and programs to be executed by the program.

## OPTIONS
    .bin/myshell [file]
    myshell only has one option when being invoked, it is an optional space for a path to a file. If this file contains commands and programs that can be interpreted and run by the shell it acts as a replacement for taking input from the user. Shell Scripting or Batch Programming as it is known can be used to run lots of commands in succession without the need for the user to type them all out.

## INVOCATION
    ./bin/myshell
    After being invoked by the user without a batch file, the program provides an interactive environment for the user to input commands, by default the shell will then output the results of these commands to the terminal under the prompt where the user entered the command.

## CONTROL SYMBOLS
    & - Flag as Background Process
    < - signals next argument is an input file
    > - signals next argument is a filename to output to (creates the file if it does not exist, overwrites it if it does)
    >> - signals next argument is a filename to output to (creates the file if it does not exists, appends to it if it does)

## I/O REDIRECTION
    If the user wishes to change where the output of commands and programs is printed to or where the input for a program or command is fetched from they can do so using I/O redirection and it's corresponding control symbols (<, >, >>)
    INPUT REDIRECTION
    If the user inputs a < symbol after their command followed by a valid filename, the shell will look in that file for input data instead of asking the user for it in the command or asking them to input it on following lines. 
