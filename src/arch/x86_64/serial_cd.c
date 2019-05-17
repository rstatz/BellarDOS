#include <stdint.h>

#include "serial_cd.h"
#include "io.h"
#include "irq.h"
#include "pic_cd.h"

#define DLAB_MASK 0x80

#define BAUD_115200 1
#define BAUD_9600 12
#define BAUD_DIVISOR BAUD_115200

#define BITWIDTH_7 0x02
#define BITWIDTH_8 0x03
#define BITWIDTH BITWIDTH_8
#define BITWIDTH_MASK 0x03

#define ONE_STOP 0x00
#define TWO_STOP 0x04
#define STOP_BITS ONE_STOP
#define STOP_MASK 0x04

#define PARITY_NONE 0x00
#define PARITY_ODD 0x08
#define PARITY_EVEN 0x18
#define PARITY PARITY_NONE
#define PAR_MASK 0x38

#define FIFO_CTRL_CFG 0xC7

#define INT_DATA_AVAIL 0x01
#define INT_TRANS_EMPTY 0x02
#define INT_ERROR 0x04
#define INT_STATUS_CHANGE 0x08
#define SER_INT_SUPPORTED INT_TRANS_EMPTY

// Registers
#define COM1 0x3F8
#define COM1_DATA COM1
#define COM1_IE (COM1 + 1)
#define COM1_IIR (COM1 + 2)
#define COM1_LC (COM1 + 3)
#define COM1_LS (COM1 + 5)

// Line Status Register Bits
#define LS_DATA_READY 0x01
#define LS_THRE 0x20

#define IRQ_LINE_COM1 4

serial_buff s_buff;

void SER_set_divisor(uint16_t div) {
    char lc;

    // Enter set divisor mode
    lc = inb(COM1_LC);
    outb(COM1_LC, (lc | DLAB_MASK));
    io_wait();

    // Set divisor
    outb(COM1_DATA, div);
    outb(COM1_IE, div >> 8);
    io_wait();
    
    // Exit set divisor mode
    outb(COM1_LC, (lc & ~DLAB_MASK));
}

void SER_configure_lc(uint8_t bw, uint8_t stop, uint8_t par) {
    char lc, lc_new;

    lc = inb(COM1_LC);

    lc_new = ((lc & ~BITWIDTH_MASK) | bw);
    lc_new = ((lc & ~STOP_MASK) | stop);
    lc_new = ((lc & ~PAR_MASK) | par);

    outb(COM1_LC, lc_new);
}

void SER_int_enable(uint8_t ie) {
    outb(COM1_IE, ie);
}

void SER_int_disable() {
    outb(COM1_IE, 0);
}

void SER_init() {
    SER_int_disable();

    s_buff.head = &s_buff.buff[0];
    s_buff.tail = &s_buff.buff[0];
    s_buff.next = &s_buff.buff[1];

    SER_set_divisor(BAUD_DIVISOR);
    SER_configure_lc(BITWIDTH, STOP_BITS, PARITY);
    outb(COM1_IIR, FIFO_CTRL_CFG);
    SER_int_enable(SER_INT_SUPPORTED); // Double check this is correct interrupt

    IRQ_clear_mask(IRQ_LINE_COM1);
}

int is_tx_empty() {
    return inb(COM1_LS) & LS_THRE;
}

void SER_tx(unsigned char data) {
    if (is_tx_empty()) // CHECK EMPTY?
        outb(COM1_DATA, data);
}

int SER_write(char data) {
    int disabled_ints = 0;
    
    if (irq_enabled()) {
        disabled_ints = 1;
        CLI;
    }
  
    if (s_buff.head == s_buff.next)
        return 0;    

    if (s_buff.head == s_buff.tail) {
        SER_tx(data);
        return 1;
    }

    *s_buff.tail = data;
    s_buff.tail = s_buff.next;
    s_buff.next++;

    if (s_buff.next > &s_buff.buff[SERIAL_BUFF_SIZE])
        s_buff.next = &s_buff.buff[0];

    if (disabled_ints)
        STI;

    return 1;
}

void SER_write_str(const char* str) {
    int i = 0;

    while (str[i] != '\0')
        SER_write(str[i]);        
}

void IRQ_SER_tx() {
    if (s_buff.head == s_buff.next)
        return;

    SER_tx((unsigned char)*s_buff.head);
    
    s_buff.head++;
    
    if (s_buff.head > &s_buff.buff[SERIAL_BUFF_SIZE])
        s_buff.head = &s_buff.buff[0];

    inb(COM1_IIR);
}
