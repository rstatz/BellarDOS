#ifndef PIC_CD_H
#define PIC_CD_H

#define CLI asm("CLI")
#define STI asm("STI")

typedef void (*irq_handler_t)(int, int, void*);

void pic_init();

void IRQ_set_handler(int irq, irq_handler_t, void*);

#endif
