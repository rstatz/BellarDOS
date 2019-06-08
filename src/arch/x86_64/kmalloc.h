#include <stddef.h>

#ifndef KMALLOC_H
#define KMALLOC_H

typedef struct km_header {
    struct km_header* next;
    size_t size;
} km_header;

typedef struct km_footer {
    struct km_header* prev;
    size_t size;
} km_footer;

typedef struct km_pool {
    km_header* flist;
    void* brk;
} km_pool;

void kmalloc_init();

void* kmalloc(size_t);
void kfree(void* addr);

#endif
