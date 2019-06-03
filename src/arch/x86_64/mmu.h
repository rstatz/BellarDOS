#ifndef MMU_H
#define MMU_H

#define VA_FREE_STACK_SIZE 100

/************************************************************\
Virtual Memory Map:
**************************************************************
Base Address         | Usage
    0x00000000000    |     Physical Page Frames (Identity Map)
    (PML4E SLot 0)   |
    0x01000000000    |     Kernel Stacks
    (PML4E Slot 1)   |     
    0x02000000000    |     Reserved / Growth
    (PML4E Slot 2-14)|     
    0x0F000000000    |     Kernel Heap
    (PML4E Slot 15)  |
    0x10000000000    |     User Space Starts
    (PML4E Slot 16)  |
\************************************************************/

typedef struct PML4_ref {
    uint16_t reserved_0: 3;
    uint16_t pwt: 1;
    uint16_t pcd: 1;
    uint16_t reserved_1: 7;
    uint64_t base_addr: 40;
    uint16_t reserved_2: 12;
} __attribute__((packed)) PML4_ref;

typedef struct PML4E {
    uint8_t p: 1;
    uint8_t rw: 1;
    uint8_t us: 1;
    uint8_t pwt: 1;
    uint8_t pcd: 1;
    uint8_t a: 1;
    uint8_t ign: 1;
    uint16_t mbz: 2;
    uint16_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint16_t nx: 1;
} __attribute__((packed)) PML4E;

typedef struct L3E {
    uint8_t p: 1;
    uint8_t rw: 1;
    uint8_t us: 1;
    uint8_t pwt: 1;
    uint8_t pcd: 1;
    uint8_t a: 1;
    uint8_t ign: 1;
    uint8_t s: 1; 
    uint8_t mbz: 1;
    uint8_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint8_t nx: 1;
} __attribute__((packed)) L3E;

typedef struct L2E {
    uint8_t p: 1;
    uint8_t rw: 1;
    uint8_t us: 1;
    uint8_t pwt: 1;
    uint8_t pcd: 1;
    uint8_t a: 1;
    uint8_t ign: 1;
    uint8_t zero: 1;
    uint8_t ign2: 1;
    uint8_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint8_t nx: 1;
} __attribute__((packed)) L2E;

typedef struct L1E {
    uint8_t p: 1;
    uint8_t rw: 1;
    uint8_t us: 1;
    uint8_t pwt: 1;
    uint8_t pcd: 1;
    uint8_t a: 1;
    uint8_t d: 1;
    uint8_t pat: 1;
    uint8_t g: 1;
    uint8_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint8_t nx: 1;
} __attribute__((packed)) L1E;

typedef struct vaddr {
    uint16_t phys_offset: 12;
    uint16_t pt_offset: 12;
    uint16_t pd_offet: 12;
    uint16_t pdp_offset: 12;
    uint16_t pml4_offset: 12;
    uint8_t se: 4;
} __attribute__((packed)) vaddr;

typedef struct virt_map {
    PML4E* pml4e;
    uint32_t lost_mem_stat;
    uint64_t *vasp, va_last;
    uint64_t va_free_stack[VA_FREE_STACK_SIZE];
} virt_map;

void MMU_init(PML4_ref, void* tag);

void* MMU_pf_alloc();

void MMU_pf_free(void*);

void* MMU_alloc_page();

void MMU_free_page(void*);

#endif
