#include <stdint.h>

#include "io.h"
#include "idt.h"
#include "pic_cd.h"

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_CMD PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_CMD PIC2
#define PIC2_DATA (PIC2 + 1)

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10

#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM 0x10

#define VEC_OFFSET1 0x20
#define VEC_OFFSET2 0x28

// Commands
#define PIC_EOI 0x20
#define PIC_READ_IRR 0x0a // Interrupts raised
#define PIC_READ_ISR 0x0b // Interrupts being serviced

void PIC_sendEOI(uint8_t irq) {
    if (irq >= 8)
        outb(PIC2_CMD, PIC_EOI);
    
    outb(PIC1_CMD, PIC_EOI); 
}

void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8)
        port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        IRQline -= 8;
    }

    value = inb(port) | (1 << IRQline);
    outb(port, value);
}

void IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8)
        port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        IRQline -= 8;
    }

    value = inb(port) & ~(1 << IRQline);
    outb(port, value);
}

static uint16_t pic_get_irq_reg(int ocw3) {
    outb(PIC1_CMD, ocw3);
    outb(PIC2_CMD, ocw3);
    io_wait();
    
    return (inb(PIC1_CMD) << 8) | inb(PIC1_CMD);
}

uint16_t pic_get_irr() {
    return pic_get_irq_reg(PIC_READ_IRR);
}

uint16_t pic_get_isr() {
    return pic_get_irq_reg(PIC_READ_ISR);
}

void pic_remap(int offset1, int offset2) {
    uint8_t m1, m2;

    m1 = inb(PIC1_DATA);
    m2 = inb(PIC2_DATA);

    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, offset1);
    io_wait();
    outb(PIC2_DATA, offset2);
    io_wait();
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, m1);
    outb(PIC2_DATA, m2);
}

void pic_init() {
    pic_remap(VEC_OFFSET1, VEC_OFFSET2);    
}
