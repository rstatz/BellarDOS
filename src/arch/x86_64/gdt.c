#include <stdint.h>
#include "gdt.h"

#define MASK_4 0xF
#define MASK_8 0xFF
#define MASK_16 0xFFFF

#define GDT_NUM_8
#define GDT_NUM_16
#define GDT_SIZE_8BYTES (GDT_NUM_8 + (2 * GDT_NUM_16))
#define GDT_SIZE_BYTES (GDT_SIZE_8BYTES * 8)

GDTentry[GDT_SIZE_8BYTES] gdt;

GDTentry newGDTentry(uint32_t limit, uint32_t base, uint8_t access, uint8_t flags) {
    GDTentry e;

    e.limit_low = limit & MASK_16;
    e.base_low = (uint16_t)(base & MASK_16);

    e.base_mid = (uint8_t)((base >> 16) & MASK_8);
    e.access = access;
    e.limit_up = (limit >> 16) & MASK_4;
    e.flags = flags & MASK_4;
    e.base_up = (uint8_t)((base >> 24) & MASK_8);
}

void gdt_init() {

}

void gdt_load() {
    GDTdesc desc;

    desc.limit = GDT_SIZE_BYTES - 1;
    desc.offset = (uint64_t)&gdt;

    asm volatile("lgdt %0" : : "m" (desc));
}
