#include "ist.h"
#include "print.h"
#include "debug.h"

#define MASK_4 0xF
#define MASK_8 0xFF
#define MASK_16 0xFFFF
#define MASK_32 0xFFFFFFFF

#define TSS_SIZE 0x68

TSS tss;

extern void* stack_ist1_top;
extern void* stack_ist2_top;

TSSdesc new_TSSdesc() {
    TSSdesc t;

    t.zero = 0;
    
    t.reserved_1 = 0;
    t.reserved_2 = 0;

    t.g = 0;
    t.dpl = 0;
    t.avl = 1;
    t.type = 0b1001;
    t.present = 1;

    t.seg_limit_low = (TSS_SIZE - 1) & MASK_16;
    t.seg_limit_up = (TSS_SIZE >> 16) & MASK_4;

    t.base_addr_up = (((uint64_t)(&tss)) >> 32) & MASK_32;
    t.base_addr_mup = (((uint64_t)(&tss)) >> 24) & MASK_8;
    t.base_addr_mlow = (((uint64_t)(&tss)) >> 16) & MASK_8;
    t.base_addr_low = ((uint64_t)(&tss)) & MASK_16;

    return t;
}

void load_task_register(uint64_t tss_byte_offset) {
    asm volatile("ltr %0" : : "m" (tss_byte_offset));
}

void init_TSS() {
    tss.reserved_io = 0;
    tss.reserved_1 = 0;
    tss.reserved_2 = 0;
    tss.reserved_3 = 0;

    tss.io_base = 0;

    tss.rsp0 = 0;
    tss.rsp1 = 0;
    tss.rsp2 = 0;

    tss.ist1 = (uint64_t)stack_ist1_top;
    tss.ist2 = (uint64_t)stack_ist2_top;

    tss.ist3 = 0;
    tss.ist4 = 0;
    tss.ist5 = 0;
    tss.ist6 = 0;
    tss.ist7 = 0;
}
