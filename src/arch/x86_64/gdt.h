#include <stdint.h>

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
    uint8_t access;
    uint8_t limit_up: 4;
    uint8_t flags: 4;
    uint8_t base_up;
} __attribute__((packed)) GDTentry;

GDTentry newGDTentry();

void gdt_init();

void gdt_load();

#endif
