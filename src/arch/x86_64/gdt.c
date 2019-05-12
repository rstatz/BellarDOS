#include <stdint.h>
#include "gdt.h"
#include "ist.h"

#define MASK_4 0xF
#define MASK_8 0xFF
#define MASK_16 0xFFFF

#define GDT_NUM_8 3
#define GDT_NUM_16 1
#define GDT_SIZE_8BYTES (GDT_NUM_8 + (2 * GDT_NUM_16))
#define GDT_SIZE_BYTES (GDT_SIZE_8BYTES * 8)

#define TSS_DESC_SIZE_BYTES 16
#define TSS_DESC_BYTE_OFFSET 3 * sizeof(GDTentry)

#define CODE_SEGMENT_FLAGS 0xC
#define CODE_SEGMENT_ACCESS 0x9A

#define DATA_SEGMENT_FLAGS 0xC
#define DATA_SEGMENT_ACCESS 0x92

GDTentry gdt[GDT_SIZE_8BYTES];

/*GDTentry_access_byte new_GDTentry_access_byte(uint8_t rw, uint8_t exec) {
    GDTentry_access_byte a;

    a.accessed = 0;
    a.rw = rw;
    a.dc = 1;
    a.executable = exec;
    a.desc_type = 1;
    a.privl = 0;
    a.present = 1;

    return a;
}*/

GDTentry newGDTentry(uint32_t limit, uint32_t base, uint8_t access, uint8_t flags) {
    GDTentry e;

    e.limit_low = limit & MASK_16;
    e.base_low = (uint16_t)(base & MASK_16);

    e.base_mid = (uint8_t)((base >> 16) & MASK_8);
    e.access = access;
    e.limit_up = (limit >> 16) & MASK_4;
    e.flags = flags & MASK_4;
    e.base_up = (uint8_t)((base >> 24) & MASK_8);

    return e;
}

GDTentry get_gdt_code_segment_entry() {
    GDTentry e;

    e = newGDTentry(0, 0, CODE_SEGMENT_ACCESS, CODE_SEGMENT_FLAGS);

    return e;
}

GDTentry get_gdt_data_segment_entry() {
    GDTentry e;

    e = newGDTentry(0, 0, DATA_SEGMENT_ACCESS, DATA_SEGMENT_FLAGS);

    return e;
}

void gdt_init() {
    TSSdesc tss_desc = new_TSSdesc();

    init_TSS();

    gdt[0] = newGDTentry(0, 0, 0, 0);
    gdt[1] = get_gdt_code_segment_entry();
    gdt[2] = get_gdt_data_segment_entry();
    gdt[3] = ((GDTentry*)&tss_desc)[0];
    gdt[4] = ((GDTentry*)&tss_desc)[1];
}

void gdt_load() {
    GDTdesc desc;

    desc.limit = GDT_SIZE_BYTES - 1;
    desc.base = (uint64_t)&gdt;

    asm volatile("lgdt %0" : : "m" (desc));
    load_task_register(TSS_DESC_BYTE_OFFSET);
}
