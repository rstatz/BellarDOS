#include "irq.h"
#include "print.h"
#include "debug.h"
#include "pic_cd.h"
#include "ps2_cd.h"
#include "serial_cd.h"

extern void isr_unsupported();

void interrupt_handler(int irq) {
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
