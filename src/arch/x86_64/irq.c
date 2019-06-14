#include <stdint.h>
#include "irq.h"
#include "print.h"
#include "debug.h"
#include "mmu.h"
#include "multitask.h"
#include "pic_cd.h"
#include "ps2_cd.h"
#include "serial_cd.h"

#define IRQ_DF 8
#define IRQ_SSF 12
#define IRQ_GPF 13
#define IRQ_PF 14

// Devices
#define IRQ_KEYBOARD 33
#define IRQ_SERIAL 36

// System Calls
#define SYS_EXIT 115
#define SYS_YIELD 116

extern void isr_unsupported();

void interrupt_handler(int irq, uint64_t rsp) {
//    BREAK;
    PIC_sendEOI(irq - 32);

    switch(irq) {
        case(IRQ_KEYBOARD) :
            IRQ_keyboard_handler();
            break;
        case(IRQ_SERIAL) :
            IRQ_SER_tx();
            break;
        case(SYS_EXIT) :
            PROC_exit(rsp);
            break;
        case(SYS_YIELD) :
            PROC_yield(rsp);
            break;
        default :
            printk("UNSUPPORTED INTERRUPT: IRQ %d\n", irq);
            break;
    }
}

void interrupt_handler_err(int irq, int err) {
    PIC_sendEOI(irq);

    switch(irq) {
        case(IRQ_DF) :
            printk("Double Fault: Error Code%d\n", err);
            break;
        case(IRQ_SSF) :
            printk("Stack Segment Fault: Error Code%d\n", err);
            break;
        case(IRQ_GPF) :
            printk("General Protection Fault: Error Code %d\n", err);
            break;
        case(IRQ_PF) :
            printk("Page Fault: Error Code %d\n", err); 
            IRQ_pf_handler(err);
           break;
        default:
            printk("Interrupt #%d: Error Code %d\n", irq, err);
            break;
    }
}

void gen_gpf() {
    uintptr_t p = 11;
    p = ~p;
    *((int*)p) = 10;
}
