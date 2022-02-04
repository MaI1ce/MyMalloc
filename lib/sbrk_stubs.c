#ifndef SBRK_STUBS
#include "sbrk_stubs.h"
#endif

char mem[100] = {[10] = 10, [20] = 20, [30] = 30, [40] = 40, [50] = 50, [60] = 60, [70] = 70, [80] = 80, [90] = 90};

char *break_ptr = mem;

void *sbrk(intptr_t increment)
{
    if(increment == 0)
        return (void *)break_ptr;
    if(increment < 0 || increment > 99)
        return (void *)-1;
    break_ptr += increment;
    return break_ptr;
}

int _brk(void *addr)
{
    if(addr && addr <= &mem[99] && addr >= mem){
        break_ptr = addr;
        return 1;
    } else 
        return 0;
}