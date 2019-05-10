#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef void (*isr_t)();
typedef void (*irq_handler_t)(int, int, void*);

void IRQ_set_handler(int, irq_handler_t, void*);

typedef struct IDTEntry {
    uint16_t targ_offset_lower_16: 16;
    uint16_t targ_selector: 16;
    
    uint8_t ist: 3;
    uint8_t ign5: 5;
    uint8_t type: 4;
    uint8_t zero: 1;
    uint8_t dpl: 2;
    uint8_t present: 1;
    uint16_t targ_offset_upper_16: 16;

    uint32_t targ_offset_32: 32;

    uint32_t ign32: 32;    
} __attribute__((packed)) IDTEntry;

typedef struct IDTref {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) IDTref;

typedef struct IDT {
    IDTEntry divide_by_zero;
    IDTEntry debug;
    IDTEntry non_maskable_interrupt;
    IDTEntry breakpoint;
    IDTEntry overflow;
    IDTEntry bound_range_exceeded;
    IDTEntry invalid_opcode;
    IDTEntry device_not_available;
    IDTEntry double_fault;
    IDTEntry coproc_seg_overrun;
    IDTEntry invalid_tss;
    IDTEntry segment_not_present;
    IDTEntry stack_segment_fault;
    IDTEntry general_protection_fault;
    IDTEntry page_fault;
    IDTEntry reserved1;
    IDTEntry x87_floating_point;
    IDTEntry alignment_check;
    IDTEntry machine_check;
    IDTEntry simd_floating_point;
    IDTEntry reserved[9];
    IDTEntry virtualization;
    IDTEntry security_exception;
    IDTEntry reserved2;
    IDTEntry isr[224];
} __attribute__((packed)) IDT;

IDTEntry newIDTEntry(isr_t, uint8_t);

void idt_init();

void idt_load();

#endif
