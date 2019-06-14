#include <stdint.h>
#include <stddef.h>
#include "mmap.h"
#include "mmu.h"
#include "strings.h"
#include "print.h"
#include "debug.h"

#define DFLT_NUM_PGS 100

#define K_SPACE 0
#define U_SPACE 1

#define HUGE_BIT 1
#define PRESENT_BIT 1
#define DEMAND_BIT 1

#define NUM_L4_GROWTH_PAGES 13
#define KERNEL_STACK_INDEX 1
#define KERNEL_HEAP_INDEX 15

#define PGS_PER_STACK 64

#define CVRT_VA(X) ((uint64_t)(X) >> 12)
#define CVRT_ENTRY(X) ((uint64_t)(X) << 12)
#define GB_BYTES (1 << 30)

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

//    printk("Allocating pf at %p\n", pf);

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
    memset((void*)l4, 0 , sizeof(PML4E));

    l4->p = 1;
    l4->rw = 1;
    l4->us = us;
    l4->base_addr = CVRT_VA(l3);
}

void init_l3e(L3E* l3, void* l2, uint8_t s, uint8_t us) {
    memset((void*)l3, 0, sizeof(L3E));

    l3->p = 1;
    l3->rw = 1;
    l3->us = us;
    l3->s = s;
    l3->base_addr = CVRT_VA(l2);
}

void init_l3e_g(L3E_G* l3, void* l2, uint8_t s, uint8_t us) {
    memset((void*)l3, 0, sizeof(L3E_G));

    l3->p = 1;
    l3->rw = 1;
    l3->us = us;
    l3->s = s;
    l3->base_addr = CVRT_VA(l2);
}

void init_l2e(L2E* l2, void* l1, uint8_t s, uint8_t us) {
    memset((void*)l2, 0, sizeof(L2E));

    l2->p = 1;
    l2->rw = 1;
    l2->us = us;
    l2->s = s;
    l2->base_addr = CVRT_VA(l1);
}

void init_identity_paging(PML4E* l4) {
    uint64_t i;
    L3E_G* l3;

    l3 = (L3E_G*)MMU_pf_alloc();
    
    init_l4e(l4, (void*)l3, K_SPACE);
   
    for (i = 0; i < 512; i++, l3++) {
        init_l3e_g((void*)l3, (void*)(i), HUGE_BIT, K_SPACE);
    }
}

void init_kstack_paging(PML4E* l4) {
    L3E* l3 = MMU_pf_alloc();
    
    memset((void*)l3, 0, PG_SIZE);

    init_l4e(l4, l3, K_SPACE);
    return;
}

void init_growth_paging(PML4E* l4) {
    int i;
    L3E* l3;

    for (i = 0; i < 13; i++, l4++) {
        l3 = MMU_pf_alloc();

        memset((void*)l3, 0, PG_SIZE);

        init_l4e(l4, l3, K_SPACE);
    }
}

void init_kheap_paging(PML4E* l4) {
    L3E* l3 = MMU_pf_alloc();

    memset((void*)l3, 0, PG_SIZE);
    
    init_l4e(l4, (void*)l3, K_SPACE);
}

void init_uspace_paging(PML4E* l4) {
    L3E* l3 = MMU_pf_alloc();

    memset((void*)l3, 0, PG_SIZE);

    init_l4e(l4, l3, U_SPACE);
    return;
}

void vmap_setup() {
    void* pml4_base = MMU_pf_alloc();

    vmap.pml4_base = pml4_base;
    vmap.lost_mem_stat = 0;
    vmap.vakhsp = vmap.va_kheap_free_stack;
    vmap.vakssp = vmap.va_kstack_free_stack;
    vmap.va_kstack_last = KERNEL_STACK;
    vmap.va_kheap_last = KERNEL_HEAP;
}

L3E* get_l3e(vaddr* va, PML4E* l4e) {
    if (l4e->p != PRESENT_BIT)
        return NULL; 

    return &(((L3E*)CVRT_ENTRY(l4e->base_addr))[va->pdp_offset]);
}

L2E* get_l2e(vaddr* va, L3E* l3e) {
    if (l3e->p != PRESENT_BIT)
        return NULL;
    
    return &(((L2E*)CVRT_ENTRY(l3e->base_addr))[va->pd_offset]);
}

L1E* get_l1e(vaddr* va, L2E* l2e) {
    if (l2e->p != PRESENT_BIT)
        return NULL;
    
    return &(((L1E*)CVRT_ENTRY(l2e->base_addr))[va->pt_offset]);
}

void init_vaddr_space() {
    PML4_ref cr3;
    PML4E* pml4e = vmap.pml4_base;

    memset((void*)&cr3, 0, sizeof(PML4_ref));
    cr3.base_addr = CVRT_VA(vmap.pml4_base);

    init_identity_paging(pml4e);
    init_kstack_paging(++pml4e);
    init_growth_paging(++pml4e);
    pml4e += NUM_L4_GROWTH_PAGES;
    init_kheap_paging(pml4e);
    init_uspace_paging(++pml4e);
    
    set_pt(cr3);
}

void MMU_init(void* tag) {
    mmap_setup(&mmap, tag);
    init_mem_pool();
    
    vmap_setup();
    init_vaddr_space();
}

vaddr get_kstack_va() {
    vaddr va;

    if(vmap.vakssp == vmap.va_kstack_free_stack) {
        va = *(vaddr*)(&vmap.va_kstack_last);
        vmap.va_kstack_last += PG_SIZE * PGS_PER_STACK;
    }
    else {
        vmap.vakssp--;
        va = *(vaddr*)vmap.vakssp;
    }

    return va;
}

vaddr get_kheap_va() {
    vaddr va;

    if(vmap.vakhsp == vmap.va_kheap_free_stack) {
        va = *(vaddr*)(&vmap.va_kheap_last);
        vmap.va_kheap_last += PG_SIZE;
    }
    else {
        vmap.vakhsp--;
        va = *(vaddr*)vmap.vakhsp;
    }

    return va;
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
    va.va = get_kheap_va();

//    printk("Allocating va %p\n", va.va_p);

    // Gets L4 Entry
    l4 = &vmap.pml4_base[KERNEL_HEAP_INDEX];

    // Checks for L3 table
    if ((l3 = get_l3e(&va.va, l4)) == NULL) {
        printk("ERROR: VIRTUAL MEMORY NOT INITIALIZED\n");
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

void* MMU_alloc_kstack() {
    union va {
        vaddr va;
        void* va_p;
    } va;

    void *l2_base_addr, *l1_base_addr;
    int i;
    PML4E* l4;
    L3E* l3;
    L2E* l2;
    L1E* l1;

    va.va = get_kstack_va();

    // Gets L4 Entry
    l4 = &vmap.pml4_base[KERNEL_STACK_INDEX];

    // Gets L3 Entry
    if ((l3 = get_l3e(&va.va, l4)) == NULL) {
        printk("ERROR: VIRTUAL MEMORY NOT INITIALIZED\n");
        return NULL;
    }

    // Gets L2 Entry
    if ((l2 = get_l2e(&va.va, l3)) == NULL) {
        l2_base_addr = MMU_pf_alloc();
        memset((void*)l2_base_addr, 0, PG_SIZE);

        init_l3e(l3, l2_base_addr, !HUGE_BIT, K_SPACE);
        l2 = &(((L2E*)(l2_base_addr))[va.va.pd_offset]);
    }

    // Gets L1 Entry
    if ((l1 = get_l1e(&va.va, l2)) == NULL) {
        l1_base_addr = MMU_pf_alloc();
        memset((void*)l1_base_addr, 0, PG_SIZE);

        init_l2e(l2, l1_base_addr, !HUGE_BIT, K_SPACE);
        l1 = &(((L1E*)(l1_base_addr))[va.va.pt_offset]);
    }

    for (i = 0; i < PGS_PER_STACK; i++, l1++) {
        memset((void*)l1, 0, sizeof(L1E));

        l1->rw = 1;
        l1->avl_lo |= DEMAND_BIT;
    }

    return va.va_p + ((PGS_PER_STACK - 1) * PG_SIZE);
}

void MMU_free_page(void* p) {
    vaddr* va = (vaddr*)&p;
    PML4E* l4;
    L3E* l3;
    L2E* l2;
    L1E* l1;

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
        MMU_pf_free((void*)CVRT_ENTRY(l1->base_addr));
        l1->p = 0;
        l1->avl_lo &= ~DEMAND_BIT;
    }
    else if (l1->avl_lo & DEMAND_BIT)
        l1->avl_lo &= ~DEMAND_BIT;
    else {
        printk("ERROR: Unable to free va %p\n", p);
        return;
    }

    // Adds freed va space to free stack
    if ((uint64_t)p >= KERNEL_HEAP) {
        if (vmap.vakhsp == (vmap.va_kheap_free_stack + sizeof(vmap.va_kheap_free_stack))) {
            vmap.lost_mem_stat++;
            return;
        }

//        printk("Freed va at %p\n", (void*)*(uint64_t*)va);

        *vmap.vakhsp = *(uint64_t*)va;
        vmap.vakhsp++;
    }
}

void MMU_free_kstack(void* p) {
    union va {
        vaddr va;
        void* va_p;
    } va;

    int i;

    va.va_p = p;

    for (i = 0; i < PGS_PER_STACK; i++, va.va.pt_offset--) {
        MMU_free_page(va.va_p);
//        printk("Freed va at %p\n", va.va_p);
    }

    if (vmap.vakssp == (vmap.va_kstack_free_stack + sizeof(vmap.va_kstack_free_stack))) {
        vmap.lost_mem_stat++;
        return;
    }

//    printk("Freed kstack at %p\n", p);

    *vmap.vakssp = (uint64_t)p;
    vmap.vakssp++;
}

void pf_error(int err, void* va) {
    PML4_ref pt = get_pt_addr();
    
    printk("#PF ERROR %x: \n\tINVALID ADDRESS %p\n\tPAGE TABLE %p\n", 
            err, va, (void*)CVRT_ENTRY(pt.base_addr));
    HLT;
}

// TODO: COW
void IRQ_pf_handler(int err) {
    union va {
        vaddr va;
        void* va_p;
    } va;

    PML4_ref cr3;
    PML4E* l4;
    L3E* l3;
    L2E* l2;
    L1E* l1; 

    cr3 = get_pt_addr();
    va.va = get_pf_va();

    vmap.pml4_base = (void*)CVRT_ENTRY(cr3.base_addr);
    l4 = &vmap.pml4_base[va.va.pml4_offset];
   
//    printk("#PF INFO %x:\n\t VA %p\n\tPAGE TABLE %p\n", 
//        err, va.va_p, vmap.pml4_base);

    // Check if invalid address
    if ((!l4->p)
      || ((l3 = get_l3e(&va.va, l4)) == NULL)
      || ((l2 = get_l2e(&va.va, l3)) == NULL)
      || ((l1 = get_l1e(&va.va, l2)) == NULL))
        pf_error(err, va.va_p);   
 
    if (l1->avl_lo & DEMAND_BIT) {
        l1->base_addr = CVRT_VA(MMU_pf_alloc());
        l1->avl_lo &= ~DEMAND_BIT;
        l1->p = 1;
        l1->rw = 1;
    }
    else
        pf_error(err, va.va_p);
}
