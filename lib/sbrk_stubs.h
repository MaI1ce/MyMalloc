#ifndef SBRK_STUBS
#define SBRK_STUBS
#include <stdint.h>


extern char *break_ptr;

int _brk(void *addr);

void *_sbrk(intptr_t increment);

#endif