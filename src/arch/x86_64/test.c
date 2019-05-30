#include "test.h"

#include <stddef.h>

#include "mmap.h"
#include "mmu.h"
#include "debug.h"
#include "print.h"

#define PG_SIZE 4096

void fill_pf(void* pf) {
    uint32_t i;

    for (i = 0; i < (PG_SIZE/sizeof(void*)); i++)
        ((void**)pf)[i] = pf;
}

void test_pf_alloc() {
    int i;
    void* pgs[10];

    for (i = 0; i < 10; i++) {
        pgs[i] = MMU_pf_alloc();
        printk("Alloc'd pf at %p\n", pgs[i]);
    //    fill_pf(pgs[i]);
    }

    for (i = 0; i < 5; i++) {
        MMU_pf_free(pgs[i]);
        printk("Freed pf at %p\n", pgs[i]); 
   }

    pgs[0] = MMU_pf_alloc();
    printk("Alloc'd pf at %p\n", pgs[0]);
    //fill_pf(pgs[0]);
    
    for (i = 5; i < 10; i++) {
        MMU_pf_free(pgs[i]);
        printk("Freed pf at %p\n", pgs[i]);
    }

    MMU_pf_free(pgs[0]);
    printk("Freed pf at %p\n", pgs[0]);

    MMU_pf_alloc(); 
//    BREAK;
    while ((pgs[0] = MMU_pf_alloc()) != NULL)
        printk("%p\n", pgs[0]);
}
