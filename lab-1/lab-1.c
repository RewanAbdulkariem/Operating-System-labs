#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>
#include "main.h"
/**
 * Created by Rewan on 3/7/23.
 */
// Initialize global variables
char *cwd;                  // current working directory
char exit_shell = 0;        // flag to indicate if shell should exit
char* built_in[4] = {"cd", "export", "echo","exit"};  // array contains built-in commands
char ** argv = NULL;            // array of arguments passed to a command
int argc = 0;                   // number of arguments passed to a command
char *token = NULL;              // pointer to a token in a string

int main(void)
{
    FILE *f = fopen("log.txt", "w");    // open a file named "log.txt" for writing
    fprintf(f, "%s", "");               // write an empty string to the file
    fclose(f);                                 // close the file
    signal(SIGCHLD, on_child_exit);       // register a signal handler for child processes
    setup_environment();                             // initialize environment variables
    shell();                                         // start the shell
    return 0;
}
// This function is called when a child process terminates
void on_child_exit(int signal)
{
    int status;                     // variable to store the status of the child process
    pid_t child_pid;                            // variable to store the ID of the terminated child process

    FILE *f = fopen("log.txt", "a");    // open the log file in append mode
    if (f == NULL)                 // check if the file was successfully opened
    {
        perror("Error opening log file");   // print an error message
        exit(EXIT_FAILURE);             // exit the program with failure status
    }
    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0)     // loop through all terminated child processes
    {
        fprintf(f, "Child %d Terminated\n",child_pid);              // write a message to the log file indicating the termination of the child process
    }
    fclose(f); // close the log file
}
// This function sets up the environment variables
void setup_environment()
{
    cwd = getcwd(cwd,PATH_MAX);         // get the current working directory and store it in the cwd variable
    chdir(cwd);         // change the current working directory to the cwd variable
}
// This function implements the main shell loop
void shell()
{
    char *cmd = NULL ;      // pointer to the command entered by the user
    size_t n = 0;           // variable to store the size of the input buffer
    char InternalCmd;       // flag to indicate if the command is a built-in command

    while (! exit_shell)    // continue the loop until the exit_shell flag is set
    {
        InternalCmd = 0;    // reset the InternalCmd flag

        getcwd(cwd, 256);   // get the current working directory and store it in the cwd variable
        printf("%s ",cwd);    // print the current working directory
        printf("SHELL# ");     // print the prompt

        // Check if get-line() fails or the user enters an empty command
        if (getline(&cmd, &n, stdin) == -1 || cmd[0] == '\n')
            continue;       //skip to the next iteration of the loop

        parse_input(cmd," \n"); // parse the command into arguments
        for (int i = 0; i < 4;i++)
        {
            if (!strcmp(argv[0],built_in[i]))   // check if the command is a built-in command
            {
                InternalCmd = 1;            // set the InternalCmd flag
                break;                      // exit the loop
            }
            else
                InternalCmd = 0;             // reset the InternalCmd flag
        }

        if (InternalCmd == 1)           // if the command is a built-in command
            shellBultin();              // execute the built-in command
        else
            executeCommand();           // otherwise, execute the command as an external program
    }
    free(cmd),free(argv), free(cwd);    // free the memory used by the cmd, argv and cwd variables
}
void  parse_input(char *cmd,char *delim)
{
    argv = NULL;            // reset argv to null
    argc = 0;               // reset argc to 0
    char *token2 = NULL;    // initialize token2 to NULL
    token = strtok(cmd, delim); // tokenize the command string using the specified delimiter

    while (token)       // loop through all tokens
    {
        if (token[strlen(token) - 1] == '\"')   // if token ends with double quotes
            token[strlen(token) - 1] = '\0';    // remove the double quotes
        if (token[0] == '\"')                       // if token starts with double quotes
            token++;                                // remove the double quotes
        if (token[0] == '$') // if token starts with $
        {
            char* env_var = getenv(token + 1); // get value of env variable
            if (env_var != NULL) // if env variable exists
            {
                token2 = strtok(env_var," ");   // tokenize the environment variable value using space as the delimiter
                while (token2)  //loop through all tokens in the environment variable value
                {
                    argv = realloc(argv, (argc + 1) * sizeof(char *));  // increase the size of argv by one
                    argv[argc++] = token2; // add env variable value to argv array
                    token2 = strtok(NULL," ");  // get the next token in the environment variable value
                }
            }
        }
        else    // if token does not start with $
        {
            argv = realloc(argv, (argc + 1) * sizeof(char *));  // increase the size of argv by one
            argv[argc++] = token;   // add the token to the argv array and increment argc
        }
        token = strtok(NULL, delim);        // get the next token in the command string
    }
    argv = realloc(argv, (argc + 1) * sizeof(char *));      // increase the size of argv by one
    argv[argc] = NULL;          // set the last element of argv to NULL to terminate the array
    free(token2);           // free the memory allocated for token2
}
void shellBultin()
{
    if (strcmp(argv[0], "cd") == 0)     // Check if the command is "cd"
    {
        // If the second argument is not provided or is "~", set the current working directory to "/home"
        if(argv[1] == NULL || !strcmp(argv[1],"~"))
            cwd = "/home";
        else
            // Otherwise, set the current working directory to the second argument
            cwd = argv[1];
        chdir(cwd); // Change the current working directory using the chdir function
    }
    else if (strcmp(argv[0], "echo") == 0)      // Check if the command is "echo"
    {
        int i ;
        for ( i = 1; i < argc; i++)         // Loop through all arguments starting from argv[1]
        {
            // If an argument is enclosed in double quotes, remove the quotes before printing the argument
            if (argv[i][strlen(argv[i]) - 1] == '\"')
                argv[i][strlen(argv[i]) - 1] = '\0';
            if (argv[i][0] == '\"')
                argv[i]++;
            printf("%s ", argv[i]);             // Print the argument
        }

        printf("\n");           // Print a newline character to end the line
    }
    else if (strcmp(argv[0], "export") == 0)        // Check if the command is "export"
    {
        char * var = NULL;
        char * value = NULL;

        token = strtok(argv[1],"=");         // Parse the second argument to obtain the environment variable name and value
        var = token;
        token = strtok(NULL,"");
        value = malloc(strlen(token) + 1);
        strcpy(value, token);
        // If the value is enclosed in double quotes, remove the quotes before setting the value
        if (value[0] == '\"')
            value++;

        for (int i = 2; i < argc; i++)              // Concatenate any additional arguments (argv[2] and beyond) to the value, separated by spaces
        {
            strcat(value, " ");
            strcat(value, argv[i]);

        }
        // If the value is enclosed in double quotes, remove the quotes before setting the value
        if (value[strlen(value)-1] == '\"')
            value[strlen(value)-1] = '\0';

        setenv(var,value,1);            // Set the environment variable using the setenv function
    }
    else if (strcmp(argv[0], "exit") == 0)     // Check if the command is "exit"
        exit_shell = 1;                         // If the command is "exit", set the exit_shell flag to indicate that the program should exit
}
void executeCommand()
{
    // Assume it is an executable command and execute it using execvp
    int background_flag = 0;
    if (strstr(argv[argc-1],"&"))       // Check if the last argument is "&" to determine if the command should be run in the background
        background_flag = 1;

    pid_t pid = fork();         // Fork a new process to execute the command

    switch (pid)
    {
        case 0:                 // Child process
            if (execvp(argv[0], argv) == -1)                // Execute the command using the execvp function
                perror("executeCommand: ");                     // If the command cannot be executed, print an error message and exit with failure status

            exit(EXIT_FAILURE);
        case -1:                // Fork error

            perror("fork: ");
            break;
        default:
            if (!background_flag)                   // If the command is not run in the background, wait for the child process to finish
                waitpid(pid, NULL, 0);
            else
                background_flag = 0;                // Otherwise, set the background flag to 0 (to prevent it from being carried over to the next command)
            break;
    }
}