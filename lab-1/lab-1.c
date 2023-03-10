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
char* built_in[4] = {"cd", "export", "echo","exit"};
char ** argv = NULL;
int argc = 0;
char *token = NULL;

int main(void)
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
    char *cmd = NULL ;
    size_t n = 0;
    char InternalCmd;

    while (! exit_shell)
    {
        InternalCmd = 0;

        getcwd(cwd, 256);
        printf("%s ",cwd);
        printf("SHELL# ");

        // Check if get-line() fails or the user enters an empty command
        if (getline(&cmd, &n, stdin) == -1 || cmd[0] == '\n')
            continue;

        parse_input(cmd," \n");
        for (int i = 0; i < 4;i++)
        {
            if (!strcmp(argv[0],built_in[i]))
            {
                InternalCmd = 1;
                break;
            }
            else
                InternalCmd = 0;
        }

        if (InternalCmd == 1)
            shellBultin();
        else
            executeCommand();
    }
    free(cmd),free(argv);
}
void  parse_input(char *cmd,char *delim)
{
    argv = NULL;
    argc = 0;
    char *token2 = NULL;
    token = strtok(cmd, delim);
    while (token)
    {
        if (token[0] == '$') // if token starts with $
        {
            char* env_var = getenv(token + 1); // get value of env variable
            if (env_var != NULL) // if env variable exists
            {
                token2 = strtok(env_var," ");
                while (token2)
                {
                    argv = realloc(argv, (argc + 1) * sizeof(char *));
                    argv[argc++] = token2; // add env variable value to argv array
                    token2 = strtok(NULL," ");
                }
            }
        }
        else
        {
            argv = realloc(argv, (argc + 1) * sizeof(char *));
            argv[argc++] = token;
        }
        token = strtok(NULL, delim);
    }
    argv = realloc(argv, (argc + 1) * sizeof(char *));
    argv[argc] = NULL;

}
void shellBultin()
{
    if (strcmp(argv[0], "cd") == 0)
    {
        if(argv[1] == NULL || !strcmp(argv[1],"~"))
            cwd = "/home";
        else
            cwd = argv[1];
        chdir(cwd);
    }
    else if (strcmp(argv[0], "echo") == 0)
    {
        int i ;
        for ( i = 1; i < argc; i++)
        {
            if (argv[i][strlen(argv[i]) - 1] == '\"')
                argv[i][strlen(argv[i]) - 1] = '\0';
            if (argv[i][0] == '\"')
                argv[i]++;
            printf("%s ", argv[i]);
        }

        printf("\n");
    }
    else if (strcmp(argv[0], "export") == 0)
    {
        char * var = NULL;
        char * value = NULL;

        token = strtok(argv[1],"=");
        var = token;
        token = strtok(NULL,"");
        value = malloc(strlen(token) + 1);
        strcpy(value, token);

        if (value[0] == '\"')
            value++;

        for (int i = 2; i < argc; i++)
        {
            strcat(value, " ");
            strcat(value, argv[i]);

        }
        if (value[strlen(value)-1] == '\"')
            value[strlen(value)-1] = '\0';

        setenv(var,value,1);
    }
    else if (strcmp(argv[0], "exit") == 0)
        exit_shell = 1;
}
void executeCommand()
{
    // Assume it is an executable command and execute it using execvp
    int pid = fork();
    switch (pid)
    {
        case 0:
            if (execvp(argv[0], argv) == -1)
            {
                perror("executeCommand: ");
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