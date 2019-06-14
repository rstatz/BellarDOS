#ifndef TEST_H
#define TEST_H

typedef struct MadGarbo {
    int i[2000];
    char c[500];
    void* v;        
} MadGarbo;

void test_pf_alloc(); 

void test_alloc_page();

void test_kmalloc();

void test_multitask();

#endif
