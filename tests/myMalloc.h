#ifndef MYMALLOC_H
#define MYMALLOC_H

#include <sys/types.h>
#include <stdint.h>
#ifndef SBRK_STUBS
#include <unistd.h>
#endif

extern char *break_ptr;

extern char mem[];

void reset();

void *_malloc(size_t size);

void *_calloc(size_t num, size_t size);

void _free(void *p);

void *_realloc(void *p, size_t size);

#endif