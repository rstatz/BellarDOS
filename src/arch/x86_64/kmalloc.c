#include <stdint.h>
#include "kmalloc.h"
#include "mmu.h"

#include "print.h"

// Uses PG_SIZE and KERNEL_HEAP defined in mmu.h

#define SET_AVL(X) (X | 1)
#define BLK_AVL(X) (X & 1) // Takes size, returns avl bit
#define BLK_SIZE(X) (X & ~1) // Takes size, returns size - avl

#define CALC_FOOT(X) (km_footer*)(X + (X->size/sizeof(km_header)) - 1)
#define CALC_HEAD(X) (km_header*)(X - (X->size/sizeof(km_header)) + 1)

#define SXTAL(X) (16 - (X % 16))

#define REDUCTION_TOLERANCE 128
#define MAX_SUPPORTED_SIZE 40960

km_pool pool;

void kmalloc_init() {
    km_footer* f;

    pool.flist = (km_header*)MMU_alloc_page();
    pool.brk = (void*)((char*)pool.flist + PG_SIZE);

    pool.flist->size = SET_AVL(PG_SIZE);
    pool.flist->next = pool.brk;

    f = CALC_FOOT(pool.flist);
    f->size = pool.flist->size;
    f->prev = NULL;
}

km_header* extend_pool(km_header* h) {
    km_footer* f;
    void* next = MMU_alloc_page();
    
    if (next != pool.brk) {
        printk("Memory not contiguous, shit don't work yo\n");
        return NULL;
    }

    pool.brk = (void*)((char*)pool.brk + PG_SIZE);

    h->next = pool.brk;
    h->size += PG_SIZE;

    f = CALC_FOOT(h);
    f->size = h->size;
    f->prev = NULL;

    return h;
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
    km_header *hnew;
    km_footer *f, *fnew;

    f = CALC_FOOT(h);
    f->size = f->size - tsize;

    hnew = CALC_HEAD(f);
    hnew->next = h->next;
    hnew->size = f->size;

    h->next = hnew;
    h->size = tsize;

    fnew = CALC_FOOT(h);
    fnew->size = h->size;
    fnew->prev = f->prev;
    
    f->prev = h;

    return h;
}

void unfree_block(km_header* h) {
    km_footer *f, *ftemp;

    f = CALC_FOOT(h);

    // Turn off AVL bits
    h->size = BLK_SIZE(h->size);
    f->size = h->size;

    // Readjust DLL
    if (f->prev != NULL)
        f->prev->next = h->next;
    if (h->next != pool.brk) {
        ftemp = CALC_FOOT(h->next);
        ftemp->prev = f->prev;
    }

    if ((f->prev == NULL) && (h->next == pool.brk)) {
        pool.flist = MMU_alloc_page();
        pool.brk = (void*)((char*)pool.brk + PG_SIZE);
        pool.flist->next = pool.brk;
        pool.flist->size = SET_AVL(PG_SIZE);

        ftemp = CALC_FOOT(pool.flist);
        ftemp->size = pool.flist->size;
        ftemp->prev = NULL;
    }
    else if (f->prev == NULL)
        pool.flist = h->next;
    else if (h->next == pool.brk)
        f->prev->next = pool.brk;

    h->next = NULL;
    f->prev = NULL;
}

void* kmalloc(size_t size) {
    km_footer *f;
    km_header *h;
    size_t tsize;

    if (size > MAX_SUPPORTED_SIZE)
        return NULL;
    
    tsize = sizeof(km_header) + sizeof(km_footer) + SXTAL(size);

    // Finds valid block
    h = get_block(tsize);

    // Splits block if needed
    if ((h->size - tsize) > REDUCTION_TOLERANCE)
        h = reduce_block_size(h, tsize);

    // DLL/AVL management
    unfree_block(h);

    return (void*)(h + 1);
}

km_header* merge_blocks(km_header* htop, km_header* hbot) {
    km_footer *ftop, *fbottom;

    ftop = CALC_FOOT(htop);
    fbottom = CALC_FOOT(hbot);

    //TODO

    return htop;
}

void kfree(void* mem) {
    km_header *h, *htemp;
    km_footer *f, *ftemp;

    h = ((km_header*)mem) - 1;

    if ((mem < (void*)KERNEL_HEAP) || (mem >= pool.brk) || BLK_AVL(h->size)) {
        printk("kmalloc: Invalid free\n");
        return;
    }

    f = CALC_FOOT(h);

    ftemp = (km_footer*)(h - 1);

    // Checks AVL above
    if ((h->next != pool.brk) && (BLK_AVL(ftemp->size))) {
        htemp = CALC_HEAD(ftemp);
        h = merge_blocks(htemp, h);
    }

    htemp = (km_header*)(f + 1);

    // Checks AVL below
    if ((f->prev != 0) && (BLK_AVL(htemp->size)))
        h = merge_blocks(h, htemp);
}
