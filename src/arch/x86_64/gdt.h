#include <stdint.h>

#ifndef GDT_H
#define GDT_H

typedef struct GDTdesc {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) GDTdesc;

/*typedef struct GDTentry_access_byte {
    uint8_t accessed: 1;
    uint8_t rw: 1;
    uint8_t dc: 1;
    uint8_t executable: 1;
    uint8_t desc_type: 1;
    uint8_t privl: 2;
    uint8_t present: 1;
} __attribute__((packed)) GDTentry_access;*/

typedef struct GDTentry {
    uint16_t limit_low;
    uint16_t base_low;
    
    uint8_t base_mid;
    uint8_t access: 8;
    uint8_t limit_up: 4;
    uint8_t flags: 4;
    uint8_t base_up;
} __attribute__((packed)) GDTentry;

void gdt_init();

void gdt_load();

#endif
