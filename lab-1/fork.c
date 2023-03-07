#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/**
 * main -
 */

int main()
{
    pid_t pid;
    size_t n = 10;
    char *buf = NULL;
    char *argv[]= {"/bin/ls", "-l", NULL};

    printf("BEFORE \n");
    pid = fork();
    if (pid == -1)
    {
        printf("not\n");
    }
    else if (pid == 0)
    {
        int val = execve(argv[0], argv, NULL);
        if (val == -1)
        {
            perror("Error");
        }
        printf("child");
    }
    else
    {
        wait(NULL);
        printf("Enter command ");
        getline(&buf, &n, stdin);
        system(buf);
        printf("parent ");
    }
    printf("AFTER\n");
}


