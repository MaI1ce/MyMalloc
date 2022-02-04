#include<stdio.h>

#include "lib/myMalloc.h"

int main()
{
    int *p = NULL;


    printf("malloc\n");
    p = (int *)_malloc(10 * sizeof(int));

    if((size_t)*(p - 40 / sizeof(int)) == 10  * sizeof(int))
        printf("true\n");
    else
        printf("false\n");
    printf("free\n");
    _free(p);

    return 0;
}