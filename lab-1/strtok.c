//
// Created by rewan on 3/7/23.
//
#include <string.h>
#include <stdio.h>

int str_tok()

{
    char str[] = "OS is great challenge";
    char *delim = " ";
    char *token;

    token = strtok(str,delim);
    while (token)
    {
        printf("%s\n", token);
        token = strtok(NULL,delim);
    }
}