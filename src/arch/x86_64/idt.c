#include <stdint.h>

#include "idt.h"
#include "print.h"
#include "pic_cd.h"
#include "ps2_cd.h"
#include "debug.h"

#define NULL 0

#define TARG_SEL 0x8
#define GATE_TYPE 0xE

#define MASK_32 0x00000000FFFFFFFF
#define MASK_16 0x0000FFFF

#define IDT_SIZE 256
#define IDT_SIZE_BYTES (IDT_SIZE * 16)

#define IST_NORMAL 1
#define IST_ERROR 2

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
extern void isr32();
extern void isr33();

extern void isr_unsupported();

static volatile IDT idt;

IDTEntry IRQ_set_handler_entry(IDTEntry e, void* isr) {
    e.targ_offset_32 = ((uint64_t)isr >> 32) & MASK_32;
    e.targ_offset_upper_16 = ((uint64_t)isr >> 16) & MASK_16;
    e.targ_offset_lower_16 = (uint64_t)isr & MASK_16;

    return e;
}

IDTEntry newIDTEntry(isr_t isr, uint8_t ist) {
    IDTEntry e;

    if (isr == NULL)
        e = IRQ_set_handler_entry(e, &isr_unsupported);
    else
        e = IRQ_set_handler_entry(e, (void*)isr);

    e.zero = 0;
    e.present = 1;
    
    e.targ_selector = TARG_SEL;
    e.type = GATE_TYPE;
    e.dpl = 0;
    e.ist = ist;
    e.ign5 = 0;
    e.ign32 = 0;

    return e;
}

void idt_init() {
    int i;

    idt.divide_by_zero = newIDTEntry(&isr0, IST_NORMAL);
    idt.debug = newIDTEntry(&isr1, IST_NORMAL);
    idt.non_maskable_interrupt = newIDTEntry(&isr2, IST_NORMAL);
    idt.breakpoint = newIDTEntry(&isr3, IST_NORMAL);
    idt.overflow = newIDTEntry(&isr4, IST_NORMAL);
    idt.bound_range_exceeded = newIDTEntry(&isr5, IST_NORMAL);
    idt.invalid_opcode = newIDTEntry(&isr6, IST_NORMAL);
    idt.device_not_available = newIDTEntry(&isr7, IST_NORMAL);
    idt.double_fault = newIDTEntry(&isr8, IST_ERROR);
    idt.coproc_seg_overrun = newIDTEntry(&isr9, IST_NORMAL);
    idt.invalid_tss = newIDTEntry(&isr10, IST_ERROR);
    idt.segment_not_present = newIDTEntry(&isr11, IST_ERROR);
    idt.stack_segment_fault = newIDTEntry(&isr12, IST_ERROR);
    idt.general_protection_fault = newIDTEntry(&isr13, IST_ERROR);
    idt.page_fault = newIDTEntry(&isr14, IST_ERROR);
    idt.reserved1 = newIDTEntry(&isr15, IST_NORMAL);
    idt.x87_floating_point = newIDTEntry(&isr16, IST_NORMAL);
    idt.alignment_check = newIDTEntry(&isr17, IST_ERROR);
    idt.machine_check = newIDTEntry(&isr18, IST_NORMAL);
    idt.simd_floating_point = newIDTEntry(&isr19, IST_NORMAL);
    idt.reserved[0] = newIDTEntry(&isr20, IST_NORMAL);
    idt.reserved[1] = newIDTEntry(&isr21, IST_NORMAL);
    idt.reserved[2] = newIDTEntry(&isr22, IST_NORMAL);
    idt.reserved[3] = newIDTEntry(&isr23, IST_NORMAL);
    idt.reserved[4] = newIDTEntry(&isr24, IST_NORMAL);
    idt.reserved[5] = newIDTEntry(&isr25, IST_NORMAL);
    idt.reserved[6] = newIDTEntry(&isr26, IST_NORMAL);
    idt.reserved[7] = newIDTEntry(&isr27, IST_NORMAL);
    idt.reserved[8] = newIDTEntry(&isr28, IST_NORMAL);
    idt.virtualization = newIDTEntry(&isr29, IST_NORMAL);
    idt.security_exception = newIDTEntry(&isr30, IST_ERROR);
    idt.reserved2 = newIDTEntry(&isr31, IST_NORMAL);

    for (i = 0; i < 224; i++) {
        idt.isr[i] = newIDTEntry(NULL, IST_NORMAL);
    }

    idt.isr[0] = newIDTEntry(&isr32, IST_NORMAL); // Timer
    idt.isr[1] = newIDTEntry(&isr33, IST_NORMAL); // Keyboard Interrupt
}

void idt_load() {
    IDTref ref;

    ref.limit = IDT_SIZE_BYTES - 1;
    ref.base = (uint64_t)&idt;

    asm volatile ("lidt %0" : : "m" (ref));
}

void IRQ_set_handler(int irq, isr_t isr, void* idtaddr) {
    IDTEntry* idt = (IDTEntry*)idtaddr;

    IRQ_set_handler_entry(idt[irq], isr);
}

void interrupt_handler(int irq) {
    PIC_sendEOI(irq - 32);

//    printk("Interrupt %d\n", irq);

    switch(irq) {
        case(33) :
            IRQ_keyboard_handler();
            break;
        default :
            printk("UNSUPPORTED INTERRUPT: %d\n", irq);
    }
}

void interrupt_handler_err(int irq, int err) {
    PIC_sendEOI(irq);

    printk("Interrupt #%d\n", irq);
    printk("Error Code %d\n", err);
    BREAK;
    if (irq == 13)
        BREAK;
}
