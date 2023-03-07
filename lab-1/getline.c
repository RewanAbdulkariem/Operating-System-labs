//
// Created by Rewan on 3/7/23.
//

#include <stdio.h>
#include <stdlib.h>

int get_line(void)
{
    size_t n = 10;
    char *buf = NULL;

    printf("Enter name ");
    getline(&buf, &n, stdin);

    printf("Name is %sBuffer size is %ld\n", buf, n);

    free(buf);

    return 0;
}