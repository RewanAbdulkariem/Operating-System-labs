#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "main.h"
/**
 * Created by Rewan on 3/7/23.
 */
char *cwd;
short exit_shell = 0;
int main(void)  //function parent_main()
{
    signal(SIGCHLD, on_child_exit);
    setup_environment();
    shell();
    return 0;
}
void on_child_exit(int signal)
{
    int status;
    while ((waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("Child terminated\n");
    }
}
void setup_environment()
{
    cwd = getenv("PWD");
    chdir(cwd);
}
void shell()
{
    char *cmd = NULL ,*token = NULL;
    char *delim = " \n";
    size_t n = 0;
    int argc = 0;
    char ** argv = NULL;
    while (! exit_shell)
    {
        getcwd(cwd, 256);
        printf("%s ",cwd);
        printf("SHELL# ");
        // Check if get-line() fails or the user enters an empty command
        if (getline(&cmd, &n, stdin) == -1 || cmd[0] == '\n')
        {
            continue;
        }

        argc = 0;
        argv = NULL;

        token = strtok(cmd, delim);
        while (token)
        {
            argv = realloc(argv, (argc + 1) * sizeof(char *));
            argv[argc++] = token;
            token = strtok(NULL, delim);
        }
        argv = realloc(argv, (argc + 1) * sizeof(char *));
        argv[argc] = NULL;
        execute_command(argv,argc);
    }
    free(cmd),free(argv);
}
// Check if the given command is a built-in command and execute it if it is.
// If it is not a built-in command, assume it is an executable command and execute it using execvp.
void execute_command(char **command, int arg_length)
{
    if (strcmp(command[0], "cd") == 0)
    {
        if(command[1] != NULL)
            cwd = command[1];
        else
            cwd = "/home";
        chdir(cwd);
    }
    else if (strcmp(command[0], "echo") == 0)
    {
        // Execute echo built-in command
        //execute_echo(command, arg_length);
    }
    else if (strcmp(command[0], "export") == 0)
    {
        // Execute export built-in command
        //execute_export(command, arg_length);
    }
    else if (strcmp(command[0], "exit") == 0)
        exit_shell = 1;
    else
    {
        // Assume it is an executable command and execute it using execvp
        int pid = fork();
        switch (pid)
        {
            case 0:
                if (execvp(command[0], command) == -1)
                {
                    perror("execute_command: ");
                }
                exit(EXIT_FAILURE);
            case -1:
                perror("fork: ");
                break;
            default:
                waitpid(pid, NULL, 0);
                break;
        }
    }
}