#include <stdint.h>
#include <stddef.h>
#include "mmap.h"
#include "mmu.h"
#include "strings.h"
#include "print.h"
#include "debug.h"

#define PG_SIZE 4096
#define DFLT_NUM_PGS 100

#define K_SPACE 0
#define U_SPACE 1

#define HUGE_BIT 1
#define PRESENT_BIT 1
#define DEMAND_BIT 1

#define NUM_L4_GROWTH_PAGES 13
#define KERNEL_HEAP_INDEX 15
#define KERNEL_HEAP 0x78000000000

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

void init_l4e(PML4E* l4, void* l3, uint8_t us) {
    l4->p = 1;
    l4->rw = 1;
    l4->us = us;
    l4->pwt = 0;
    l4->pcd = 0;
    l4->a = 0;
    l4->ign = 0;
    l4->mbz = 0;
    l4->avl_lo = 0;
    l4->base_addr = (uint64_t)l3;
    l4->avl_hi = 0;
    l4->nx = 0; 
}

void init_l3e(L3E* l3, void* l2, uint8_t s, uint8_t us) {
    l3->p = 1;
    l3->rw = 1;
    l3->us = us;
    l3->pwt = 0;
    l3->pcd = 0;
    l3->a = 0;
    l3->ign = 0;
    l3->s = s;
    l3->mbz = 0;
    l3->avl_lo = 0;
    l3->base_addr = (uint64_t)l2;
    l3->avl_hi = 0;
    l3->nx = 0;
}

void init_l2e(L2E* l2, void* l1, uint8_t s, uint8_t us) {
    l2->p = 1;
    l2->rw = 1;
    l2->us = us;
    l2->pwt = 0;
    l2->pcd = 0;
    l2->a = 0;
    l2->ign = 0;
    l2->s = s;
    l2->ign2 = 0;
    l2->avl_lo = 0;
    l2->base_addr = (uint64_t)l1;
    l2->avl_hi = 0;
    l2->nx = 0;
}

void init_identity_paging(PML4E* l4) {
    uint64_t i;
    void* l3;

    l3 = MMU_pf_alloc();
    
    init_l4e(l4, l3, K_SPACE);
   
    for (i = 0; i < 512; i++, l3++) {
        init_l3e(l3, (void*)(i * GB_BYTES), HUGE_BIT, K_SPACE);
    }
}

void init_kstack_paging(PML4E* l4) {
//    init_l4e(l4, NULL/*TODO (L3)*/, K_SPACE);
    return;
}

void init_growth_paging(PML4E* l4) {
    int i;
    void* va;

    for (i = 0; i < 13; i++, l4++) {
        va = MMU_pf_alloc();
        init_l4e(l4, va, K_SPACE);
    }
}

void init_kheap_paging(PML4E* l4) {
    L3E* l3 = MMU_pf_alloc();

    memset((void*)l3, 0, PG_SIZE);
    
    init_l4e(l4, (void*)l3, K_SPACE);
}

void init_uspace_paging(PML4E* l4) {
//    init_l4e(l4, NULL/*TODO (L3)*/, U_SPACE);
    return;
}

void vmap_setup(PML4_ref pml) {
    PML4E* pml4e = (PML4E*)(uint64_t)pml.base_addr;

    vmap.pml4_base = pml4e;
    vmap.lost_mem_stat = 0;
    vmap.vasp = vmap.va_free_stack;
    vmap.va_last = KERNEL_HEAP;
}

L3E* get_l3e(vaddr* va, PML4E* l4e) {
    if (l4e->p != PRESENT_BIT)
        return NULL; 

    return &(((L3E*)(uint64_t)l4e->base_addr)[va->pdp_offset]);
}

L2E* get_l2e(vaddr* va, L3E* l3e) {
    if (l3e->p != PRESENT_BIT)
        return NULL;
    
    return &(((L2E*)(uint64_t)l3e->base_addr)[va->pd_offset]);
}

L1E* get_l1e(vaddr* va, L2E* l2e) {
    if (l2e->p != PRESENT_BIT)
        return NULL;
    
    return &(((L1E*)(uint64_t)l2e->base_addr)[va->pt_offset]);
}

void init_vaddr_space() {
    PML4E* pml4e = vmap.pml4_base;

    init_identity_paging(pml4e);
    init_kstack_paging(++pml4e); // Not complete
    init_growth_paging(++pml4e); // Not complete
    pml4e += NUM_L4_GROWTH_PAGES;
    init_kheap_paging(pml4e);
    init_uspace_paging(++pml4e);
}

void MMU_init(PML4_ref pml, void* tag) {
    mmap_setup(&mmap, tag);
    init_mem_pool();
    
    vmap_setup(pml);
    init_vaddr_space();
}

void* MMU_alloc_page() {
    union va {
        vaddr va;
        void* va_p;
    } va;

    void *l2_base_addr, *l1_base_addr;
    PML4E* l4;
    L3E* l3;
    L2E* l2;
    L1E* l1;

    // Determines next va
    if(vmap.vasp == vmap.va_free_stack) {
        va.va = *(vaddr*)(&vmap.va_last);
        vmap.va_last += PG_SIZE;
    }
    else {
        vmap.vasp--;
        va.va = *(vaddr*)vmap.vasp;
    }

    printk("Allocating va %p\n", va.va_p);

    // Gets L4 Entry
    l4 = &vmap.pml4_base[KERNEL_HEAP_INDEX];

    // Checks for L3 table
    if ((l3 = get_l3e(&va.va, l4)) == NULL) {
        printk("ERROR: KERNEL HEAP NOT INITIALIZED\n");
        return NULL;
    }
  
    // Checks for L2 table
    if ((l2 = get_l2e(&va.va, l3)) == NULL) {
        l2_base_addr = MMU_pf_alloc();
        memset((void*)l2_base_addr, 0, PG_SIZE);

        init_l3e(l3, l2_base_addr, !HUGE_BIT, K_SPACE);
        l2 = &(((L2E*)(l2_base_addr))[va.va.pd_offset]);
    }

    // Checks for L1 Table
    if ((l1 = get_l1e(&va.va, l2)) == NULL) {
        l1_base_addr = MMU_pf_alloc();
        memset((void*)l1_base_addr, 0, PG_SIZE);

        init_l2e(l2, l1_base_addr, !HUGE_BIT, K_SPACE);
        l1 = &(((L1E*)(l1_base_addr))[va.va.pt_offset]);
    }

    memset((void*)l1, 0, sizeof(L1E));

    l1->rw = 1;
    l1->avl_lo |= DEMAND_BIT;

    return va.va_p;
}

void MMU_free_page(void* p) {
    vaddr* va = (vaddr*)&p;
    PML4E* l4;
    L3E* l3;
    L2E* l2;
    L1E* l1;
//    BREAK;

    // Navigates pages
    l4 = &vmap.pml4_base[va->pml4_offset];

    if ((!l4->p)
      || ((l3 = get_l3e(va, l4)) == NULL)
      || ((l2 = get_l2e(va, l3)) == NULL)
      || ((l1 = get_l1e(va, l2)) == NULL)) {
        printk("ERROR: Unable to free va %p\n", p);
        return;
    }

    // Frees page frame if present
    if (l1->p == 1) {
        MMU_pf_free((void*)(uint64_t)l1->base_addr);
        l1->p = 0;
    }

    l1->avl_lo &= ~DEMAND_BIT;

    // Adds freed va space to free stack
    if (vmap.vasp == (vmap.va_free_stack + sizeof(vmap.va_free_stack))) {
        vmap.lost_mem_stat++;
        return;
    }

    printk("Freed va at %p\n", (void*)*(uint64_t*)va);

    *vmap.vasp = *(uint64_t*)va;
    vmap.vasp++;
}

void pf_error(int err, void* va) {
    void* pt = get_pt_addr();
    
    printk("#PF ERROR %x: \n\tINVALID ADDRESS %p\n\tPAGE TABLE %p\n", 
            err, va, pt);
   HLT;
}

void IRQ_pf_handler(int err) {
    union va {
        vaddr va;
        void* va_p;
    } va;

//    BREAK;

    void* pt;;

    PML4E* l4;
    L3E* l3;
    L2E* l2;
    L1E* l1; 

    va.va = get_pf_va();
    l4 = &vmap.pml4_base[va.va.pml4_offset];
   
    pt = get_pt_addr();
    printk("#PF INFO %x:\n\t VA %p\n\tPAGE TABLE %p\n", 
        err, va.va_p, pt);
    
//    BREAK;

    if ((!l4->p)
      || ((l3 = get_l3e(&va.va, l4)) == NULL)
      || ((l2 = get_l2e(&va.va, l3)) == NULL)
      || ((l1 = get_l1e(&va.va, l2)) == NULL))
        pf_error(err, va.va_p);   
 
    if (l1->avl_lo & DEMAND_BIT) {
        l1->base_addr = (uint64_t)MMU_pf_alloc();
        l1->avl_lo &= ~DEMAND_BIT;
        l1->p = 1;
        l1->rw = 1;
    }
    else
        pf_error(err, va.va_p);

    BREAK;
}
