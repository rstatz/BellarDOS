#ifndef KMALLOC_H
#define KMALLOC_H

struct km_pool {
    int max_size;
    int avl;
    void* head;
} km_pool;

struct km_header {
    km_pool *pool;
    size_t size;
} km_header;

#endif
