#include <stdint.h>
#include "kmalloc.h"
#include "mmu.h"

#include "print.h"

// Uses PG_SIZE and KERNEL_HEAP defined in mmu.h

#define SET_AVL(X) (X | 1)
#define BLK_AVL(X) (X & 1) // Takes size, returns avl bit
#define BLK_SIZE(X) (X & ~1) // Takes size, returns size - avl

#define CALC_FOOT(X) (km_footer*)(X + (X->size/sizeof(km_header)) - 1)
#define CALC_HEAD(X) (X - (X->size/sizeof(km_header)) + 1)

#define SXTAL(X) (16 - (X % 16))

#define REDUCTION_TOLERACE 128
#define MAX_SUPPORTED_SIZE 40960

km_pool pool;

void kmalloc_init() {
    km_footer* f;

    pool.flist = (km_header*)MMU_alloc_page();
    pool.brk = pool.flist + (PG_SIZE/sizeof(void*));

    pool.flist->size = SET_AVL(PG_SIZE);
    pool.flist->next = pool.brk;

    f = CALC_FOOT(pool.flist);
    f->size = pool.flist->size;
    f->prev = 0;
}

km_header* extend_pool(km_header* h) {

}

km_header* get_block(size_t tsize) {
    km_header* h;

    h = pool.flist;

    while(BLK_SIZE(h->size) < tsize) {
        if (h->next == pool.brk)
            h = extend_pool(h);
        else
            h = h->next;
    }

    return h;
}

km_header* reduce_block_size(km_header* h, size_t tsize) {

}

void* kmalloc(size_t size) {
    km_footer *f, *ftemp;
    km_header *h;
    size_t tsize;

    if (size > MAX_SUPPORTED_SIZE)
        return NULL;
    
    tsize = sizeof(km_header) + sizeof(km_footer) + SXTAL(size);

    // Finds valid block
    h = get_block(tsize);

    // Splits block if needed
    h = reduce_block_size(h, tsize);

    // DLL management

    // AVL bit management

    return (void*)(h + 1);
}
