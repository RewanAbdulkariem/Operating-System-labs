//
// Created by rewan on 3/7/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int arg(int argc, char *argv[])
{
    printf("argc %d\n",argc);

    for (int i = 0 ; argv[i]; i++)
    {
        printf("argv[%d] is %s\n",i,argv[i]);
    }
}