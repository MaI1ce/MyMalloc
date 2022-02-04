#ifndef MYMALLOC_H
#include "myMalloc.h"
#endif

#ifndef SBRK_STUBS

int _brk(void *addr)
{
    return brk(addr);
}

void *_sbrk(intptr_t increment)
{
    return sbrk(increment);
}
#endif

#define align8(x) (((((x)-1)>>3)<<3)+8)

struct mtd_block{
    size_t size;
    struct mtd_block *next;
    struct mtd_block *prev;
    long int free;
    /*ptr is a magic number - ptr == data*/
    void *ptr;
    char data[1];
};

typedef struct mtd_block *mtd_block_ptr;

#define BLOCK_SIZE (sizeof(struct mtd_block*)*2) + sizeof(size_t) + sizeof(long int) + sizeof(void *)

static mtd_block_ptr base = NULL;

static mtd_block_ptr find_block(mtd_block_ptr *last, size_t size)
{
    mtd_block_ptr b = base;
    while (b && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
    }
    return (b);
}

static mtd_block_ptr extend_heap(mtd_block_ptr last , size_t s)
{
    mtd_block_ptr b;
    b = (mtd_block_ptr )_sbrk(0);
    if (_sbrk(BLOCK_SIZE + s) == (void*)-1)
        /* sbrk fails , go to die */
        return NULL;
    b->size = s;
    b->next = NULL;
    b->prev = last;
    b->ptr = b->data;
    if(last)
        last->next = b;
    b->free = 0;
    return (b);
}

static void split_block(mtd_block_ptr b, size_t s)
{
    mtd_block_ptr _new;
    size_t b_size = BLOCK_SIZE;
    _new = (mtd_block_ptr)(b->data + s);
    _new->size = b->size - s - b_size;
    _new->next = b->next;
    _new->prev = b;
    _new->free = 1;
    _new->ptr = _new->data;
    b->size = s;
    b->next = _new;
}

static mtd_block_ptr get_block(void *p)
{
    char *tmp = (char *)p;
    tmp -= BLOCK_SIZE;
    p = tmp;
    return (mtd_block_ptr)p;
}

/* Valid addr for free */
static int valid_addr(void *p)
{
    if(base)
        if(p > base && p < _sbrk(0))
            return (p == (get_block(p))->ptr);
    return (0);
}

static void copy_block(mtd_block_ptr src, mtd_block_ptr dst)
{
    long int *sdata;
    long int *ddata;
    size_t i;
    sdata = (long int *)src->ptr;
    ddata = (long int *)dst->ptr;
    for (i=0; i*sizeof(size_t) < src->size && i*sizeof(size_t) < dst->size; i++)
        ddata[i] = sdata[i];
}

static mtd_block_ptr fusion(mtd_block_ptr b)
{
    if(b->next && b->next->free){
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next ->next;
        if(b->next)
            b->next->prev = b;
    }
    return (b);
}

void *_malloc(size_t size)
{
    mtd_block_ptr b;
    mtd_block_ptr last;
    size_t s;
    if(size <= 0)
        return NULL;
    s = align8(size);
    if (base) {
        /* First find a block */
        last = base;
        b = find_block(&last ,s);
        if (b) {
            /* can we split */
            if ((b->size - s) >= (BLOCK_SIZE + sizeof(size_t)))
                split_block(b,s);
            b->free=0;
        } else {
            /* No fitting block , extend the heap */
            b = extend_heap(last ,s);
            if (!b)
                return(NULL);
        }
    } else {
        /* first time */
        b = extend_heap(NULL ,s);
        if (!b)
            return(NULL);
        base = b;
    }
    return(b->data);
}


void *_calloc(size_t num, size_t size)
{
    size_t *_new;
    size_t s8;
    size_t i;
    _new = (size_t *)_malloc(num * size);
    if (_new) {
        s8 = num * size >> 3;
        for (i=0; i<s8 ; i++)
        _new[i] = 0;
    }
    return (_new);
}

void _free(void *p)
{
    mtd_block_ptr b;
    if(valid_addr(p)){
        b = get_block(p);
        b->free = 1;
        /* fusion with previous if possible */
        if(b->prev && b->prev->free)
            b = fusion(b->prev);
        /* then fusion with next */
        if(b->next)
            fusion(b);
        else {
            if(b->prev)
                b->prev->next = NULL;
            else
                base = NULL;
            _brk(b);
        }
    }
}

void *_realloc(void *p, size_t size)
{
    size_t s;
    mtd_block_ptr b;
    mtd_block_ptr _new;
    void *newp;
    if(!p)
        return (_malloc(size));
    if(valid_addr(p)){
        s = align8(size);
        b = get_block(p);
        if (b->size >= s){
            if (b->size - s >= (BLOCK_SIZE + 8))
                split_block(b,s);
        } else {
            /* Try fusion with next if possible */
            if(b->next && b->next->free
                && (b->size + BLOCK_SIZE + b->next->size) >= s){
                fusion(b);
                if (b->size - s >= (BLOCK_SIZE + 8))
                    split_block(b,s);
            } else {
                /*realloc with a new block */
                newp = _malloc(s);
                if(!newp)
                    return (NULL);
                _new = get_block(newp);
                /* Copy data */
                copy_block(b,_new);
                /* free the old one */
                _free(p);
                return (newp);
            }
        }
        return (p);
    }
    return (NULL);
}