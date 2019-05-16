#include <stdint.h>
#include "ist.h"

#ifndef GDT_H
#define GDT_H

typedef struct GDTdesc {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) GDTdesc;

typedef struct GDTentry {
    uint16_t limit_low;
    uint16_t base_low;
    
    uint8_t base_mid;
    uint8_t access: 8;
    uint8_t limit_up: 4;
    uint8_t flags: 4;
    uint8_t base_up;
} __attribute__((packed)) GDTentry;

typedef struct GDT {
    uint64_t null_entry;
    GDTentry code_segment_entry;
    TSSdesc tss_desc;
} __attribute__((packed)) GDT;

void gdt_init();

void gdt_load();

#endif
