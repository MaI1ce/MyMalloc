#include<stdio.h>

#include "myMalloc.h"
#define SBRK_STUBS
#define align8(x) (((((x)-1)>>3)<<3)+8)
int main(void)
{
    char *ptr7 = (char *)_malloc(40);
    char *ptr8 = (char *)_malloc(40);
    ptr7 = (char *)_realloc(NULL, 48);

    if(break_ptr == mem + 248)
        printf("true\n");
    else
        printf("false\n");

    
    return 0;
}