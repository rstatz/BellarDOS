#include "irq.h"
#include "print.h"
#include "debug.h"
#include "pic_cd.h"
#include "ps2_cd.h"
#include "serial_cd.h"

extern void isr_unsupported();

void interrupt_handler(int irq) {
//    BREAK;
    PIC_sendEOI(irq - 32);

//    printk("Interrupt %d\n", irq);

    switch(irq) {
        case(33) :
            IRQ_keyboard_handler();
            break;
        case(36) :
            IRQ_SER_tx();
            break;
        default :
            printk("UNSUPPORTED INTERRUPT: %d\n", irq);
            break;
    }
}

void interrupt_handler_err(int irq, int err) {
    PIC_sendEOI(irq);

    switch(irq) {
        case(8) :
            printk("Double Fault: Error Code%d\n", err);
            break;
        case(12) :
            printk("Stack Segment Fault: Error Code%d\n", err);
            break;
        case(13) :
            printk("General Protection Fault: Error Code %d\n", err);
            break;
        case(14) :
            printk("Page Fault: Error Code %d\n", err);
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
