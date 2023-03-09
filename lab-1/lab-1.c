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
    char *cwd = getenv("PWD");
    chdir(cwd);
    printf("%s ",cwd);
}
void shell()
{
    short exit_shell = 0;
    char *cmd = NULL ,*token = NULL;
    char *delim = " \n";
    size_t n = 0;
    int argc = 0;
    char ** argv = NULL;
    while (! exit_shell)
    {
        printf("SHELL# ");
        getline(&cmd, &n, stdin);
        printf("%s",cmd);
        token = strtok(cmd, delim);
        while (token)
        {
            argv = realloc(argv, (argc + 1) * sizeof(char *));
            argv[argc++] = token;
            token = strtok(NULL, delim);
        }
        printf("token");
        argv = realloc(argv, (argc + 1) * sizeof(char *));
        argv[argc] = NULL;
        printf("command");
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
        printf("if");

        // Execute cd built-in command
        //execute_cd(command, arg_length);
    }
    else if (strcmp(command[0], "echo") == 0)
    {
        printf("else if 2");

        // Execute echo built-in command
       //execute_echo(command, arg_length);
    }
    else if (strcmp(command[0], "export") == 0)
    {
        printf("elseif 3");

        // Execute export built-in command
        //execute_export(command, arg_length);
    }
    else
    {
        printf("else");
        // Assume it is an executable command and execute it using execvp
        int pid = fork();
        printf("%d",pid);
        char *temp = "/bin/";
        strcat(temp,command[0]);
        strcpy(command[0],temp);
        free(temp);
        printf("%s",command[0]);
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