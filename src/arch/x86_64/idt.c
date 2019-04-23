#include <stdint.h>

#include "idt.h"

#define MASK_32 0x00000000FFFFFFFF
#define MASK_16 0x0000FFFF

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void isr_unsupported();

static IDT idt;

IDTEntry IRQ_set_handler_entry(IDTEntry e, void* isr) {
    e.targ_offset_32 = ((uint64_t)isr >> 32) & MASK_32;
    e.targ_offset2_16 = ((uint64_t)isr >> 16) & MASK_16;
    e.targ_offset_16 = (uint64_t)isr & MASK_16;

    return e;
}

IDTEntry newIDTEntry(irq_handler_t isr) {
    IDTEntry e;

    e = IRQ_set_handler_entry(e, (void*)isr);

    e.zero = 0;
    e.present = 1;

    return e;
}

void initIDT() {
    idt.divide_by_zero = newIDTEntry(isr0);
}

void loadIDT(IDT* idt) {
    asm volatile ("lidt %0"
                  : : "Nd" (idt));
}

void IRQ_set_handler(int irq, irq_handler_t isr, void* idtaddr) {
    IDTEntry* idt = (IDTEntry*)idtaddr;

    IRQ_set_handler_entry(idt[irq], isr);
}
