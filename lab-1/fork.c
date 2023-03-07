#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/**
 * main -
 */
/*
int main()
{
    pid_t pid;
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
        printf("child");
    }
    else
    {
        wait(NULL);
        printf("parent ");
    }
    printf("AFTER\n");
}


*/