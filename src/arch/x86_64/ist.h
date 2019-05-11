#include <stdint.h>

#ifndef IST_H
#define IST_H

typedef struct TSSdesc {
    uint16_t seg_limit_low: 16;
    uint16_t base_addr_low: 16;

    uint8_t base_addr_mlow: 8;
    uint8_t type: 4;
    uint8_t zero: 1;
    uint8_t dpl: 2;
    uint8_t present: 1;
    uint8_t seg_limit_up: 4;
    uint8_t avl: 1;
    uint8_t reserved_1: 2;
    uint8_t g: 1;
    uint8_t base_addr_mup: 8;

    uint32_t base_addr_up: 32;

    uint32_t reserved_2: 32;
} __attribute__((packed)) TSSdesc;

typedef struct TSS {
    uint64_t reserved_3;
     
    uint64_t rsp0; 
    uint64_t rsp1;
    uint64_t rsp2;

    uint64_t reserved_2;

    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;

    uint64_t reserved_1;

    uint16_t io_base;
    uint16_t reserved_io;
} __attribute__((packed)) TSS;

void load_task_register(uint64_t);

TSSdesc new_TSSdesc();

void init_TSS();

#endif
