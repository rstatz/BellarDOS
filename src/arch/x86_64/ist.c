#include "ist.h"
#include "debug.h"

#define MASK_8 0xFF
#define MASK_16 0xFFFF

TSS tss;

extern void* stack_ist1_top;
extern void* stack_ist2_top;

TSSdesc new_TSSdesc() {
    TSSdesc t;

    t.zero = 0;
    t.zeros = 0;

    t.reserved_1 = 0;
    t.reserved_2 = 0;
    t.reserved_3 = 0;

    t.base_addr_up = (((uint64_t)(&tss)) >> 16) & MASK_16;
    t.base_addr_mup = (((uint64_t)(&tss)) >> 24) & MASK_8;
    t.base_addr_mlow = (((uint64_t)(&tss)) >> 16) & MASK_8;
    t.base_addr_low = ((uint64_t)(&tss)) & MASK_16;

    return t;
}

void load_task_register(uint64_t tss_byte_offset) {
    BREAK;
    asm volatile("ltr %0" : : "m" (tss_byte_offset));
}

void init_TSS() {
    tss.reserved_io = 0;
    tss.reserved_1 = 0;
    tss.reserved_2 = 0;
    tss.reserved_3 = 0;

    tss.ist1 = (uint64_t)stack_ist1_top;
    tss.ist2 = (uint64_t)stack_ist2_top;
}
