#ifndef MMU_H
#define MMU_H

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

typedef struct PDPE {
    uint8_t p: 1;
    uint8_t rw: 1;
    uint8_t us: 1;
    uint8_t pwt: 1;
    uint8_t pcd: 1;
    uint8_t a: 1;
    uint8_t ign: 1;
    uint8_t zero: 1; 
    uint8_t mbz: 1;
    uint8_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint8_t nx: 1;
} __attribute__((packed)) PDPE;

typedef struct PDE {
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
} __attribute__((packed)) PDE;

typedef struct PTE {
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
} __attribute__((packed)) PTE;

typedef struct vaddr {
    uint16_t phys_offset: 12;
    uint16_t pt_offset: 12;
    uint16_t pd_offet: 12;
    uint16_t pdp_offset: 12;
    uint16_t pml4_offset: 12;
    uint8_t se: 4;
} __attribute__((packed)) vaddr;

void MMU_init(PML4_ref, void* tag);

void* MMU_pf_alloc();

void MMU_pf_free(void*);

vaddr MMU_alloc_page();

vaddr MMU_alloc_pages(int num);

void MMU_free_page(vaddr);

void MMU_free_pages(vaddr, int num);

#endif
