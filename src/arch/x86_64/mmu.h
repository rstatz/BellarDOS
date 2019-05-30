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
    uint16_t settings: 7;
    uint16_t mbz: 2;
    uint16_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint16_t nx: 1;
} __attribute__((packed)) PML4_entry;

typedef struct PDPE {
    uint8_t settings: 8;
    uint8_t mbz: 1;
    uint8_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint8_t nx: 1;
} __attribute__((packed)) PDP_entry;

typedef struct PDE {
    uint8_t settings: 8;
    uint8_t ign: 1;
    uint8_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint8_t nx: 1;
} __attribute__((packed)) PD_entry;

typedef struct PTE {
    uint8_t settings: 8;
    uint8_t g: 1;
    uint8_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint8_t nx: 1;
} __attribute__((packed)) PT_entry;

void MMU_init(void* tag);

void* MMU_pf_alloc();

void MMU_pf_free(void*);

#endif
