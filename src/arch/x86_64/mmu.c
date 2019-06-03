#include <stdint.h>
#include <stddef.h>
#include "mmap.h"
#include "mmu.h"
#include "print.h"

#define PG_SIZE 4096
#define DFLT_NUM_PGS 100

#define KERNEL_HEAP 0xF000000000

#define GB_BYTES 0x40000000

mem_map mmap;
virt_map vmap;

int add_pf() {
    mem_region r;
    uint64_t end;

    // Computes end of available region
    r = mmap.mem_avl[mmap.curr_region];
    end = r.base_addr + r.length - 1;
    end = ((end % PG_SIZE) == 0) ? end : (end + (end % PG_SIZE));

    // Automatically ignores page starting at zero
    mmap.pg_last += PG_SIZE;

    // check if room for a page in current region
    while ((mmap.pg_last >= (void*)end) 
            || ((mmap.pg_last + PG_SIZE) > (void*)end)) {
        mmap.curr_region++;

        if (mmap.curr_region >= mmap.num_avl)
            return 0;

        r = mmap.mem_avl[mmap.curr_region];
        end = r.base_addr + r.length - 1;
        end = ((end % PG_SIZE) == 0) ? end : (end + (end % PG_SIZE));
        mmap.pg_last = (void*)(r.base_addr + PG_SIZE - (r.base_addr % PG_SIZE));
    }

    if (mmap.curr_region >= mmap.num_avl)
        return 0; 

    ((pf_header*)mmap.pg_last)->next = mmap.pgs_free;
    mmap.pgs_free = mmap.pg_last;
    mmap.pgs_avl++;

    return 1;
}

void init_mem_pool() {
    mmap.pg_last = (void*)mmap.mem_avl[0].base_addr;

    while (mmap.pgs_avl != DFLT_NUM_PGS) {
        if(!add_pf())
            break;
    }
}

void* MMU_pf_alloc() {
    void* pf;

    // return NULL if no pages left to allocate
    if (!mmap.pgs_avl && !add_pf()) {
        printk("OUT OF MEMORY\n");
        return NULL;
    }

    pf = mmap.pgs_free;
    mmap.pgs_free = ((pf_header*)mmap.pgs_free)->next;
    mmap.pgs_avl--;
    mmap.pgs_alloc++;

    return pf;
}

void MMU_pf_free(void* pf) {
    ((pf_header*)pf)->next = mmap.pgs_free;
    mmap.pgs_free = pf;
    mmap.pgs_alloc--;
    mmap.pgs_avl++;
}

void init_identity_paging(PML4E* ide) {
    int i;
    L3E* l3;

    l3 = MMU_pf_alloc();

    ide->p = 1;
    ide->rw = 1;
    ide->us = 0;
    ide->pwt = 0;
    ide->pcd = 0;
    ide->a = 0;
    ide->ign = 0;
    ide->mbz = 0;
    ide->avl_lo = 0;
    ide->base_addr = (uint64_t)l3;
    ide->avl_hi = 0;
    ide->nx = 0;
    
    for (i = 0; i < 512; i++, l3++) {
        l3->p = 1;
        l3->rw = 1;
        l3->us = 0;
        l3->pwt = 0;
        l3->pcd = 0;
        l3->a = 0;
        l3->ign = 0;
        l3->s = 1; // Sets to 1Gb pages
        l3->mbz = 0;
        l3->avl_lo = 0;
        l3->base_addr = i * GB_BYTES;
        l3->avl_hi = 0;
        l3->nx = 0;
    }
}

void init_kstack_paging(PML4E* l4) {
    l4->p = 1;
    l4->rw = 1;
    l4->us = 0;
    l4->pwt = 0;
    l4->pcd = 0;
    l4->a = 0;
    l4->ign = 0;
    l4->mbz = 0;
    l4->avl_lo = 0;
//    l4->base_addr = ; Figure this out
    l4->avl_hi = 0;
    l4->nx = 0;
}

void init_growth_paging(PML4E* l4) {
    int i;

    for (i = 0; i < 13; i++, l4++) {
        l4->p = 1;
        l4->rw = 1;
        l4->us = 0;
        l4->pwt = 0;
        l4->pcd = 0;
        l4->a = 0;
        l4->ign = 0;
        l4->mbz = 0;
        l4->avl_lo = 0;
//        l4->base_addr = ; Figure this out
        l4->avl_hi = 0;
        l4->nx = 0;
    }
}

void init_kheap_paging(PML4E* l4) {

}

void init_uspace_paging(PML4E* l4) {

}

void vmap_setup(PML4_ref pml) {
    PML4e* pml4e = (PML4E*)(uint64_t)pml.base_addr;

    vmap.pml4e = pml4e;
    vmap.vasp = vmap.v_free_stack;
    vmap.va_last = KERNEL_HEAP;
}

void init_vaddr(PML4_ref pml) {
    PML4E* pml4e = vmap.pml4e;

    init_identity_paging(pml4e);
    init_kstack_paging(++pml4e); // Not complete
    init_growth_paging(++pml4e); // Not complete
    pml4e += 13;
    init_kheap_paging(pml4e);
    init_uspace_paging(++pml4e);
}

void MMU_init(PML4_ref pml, void* tag) {
    mmap_setup(&mmap, tag);
    init_mem_pool();
    
    vmap_setup(pml);
    init_vaddr();
}

void* MMU_alloc_page() {
    void* va;

    if(vmap.vasp == vmap.va_free_stack) {
        va = va_last;
        vmap.va_last += PG_SIZE;
    }
    else {
        vmap.vasp--;
        va = *vmap.vast;
    }

    // TODO: Navigate page tables and set demand bit

    return va;
}

void MMU_free_page(void* p) {
    vaddr* va = &p;

    // TODO: navigate page tables, clear demand/present bits if valid (error if not)
    // free page frame if present?

    if (vmap.vasp != (vmap.va_free_stack + sizeof(vmap.va_free_stack))) {
        *vmap.vasp = *va;
        vmap.vasp++;
        return;
    }
    
    vmap.lost_mem_stat++;
}
