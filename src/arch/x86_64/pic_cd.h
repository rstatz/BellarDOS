#ifndef PIC_CD_H
#define PIC_CD_H

#include <stdint.h>

void pic_init();

void PIC_sendEOI(uint8_t);

void IRQ_clear_mask(uint8_t IRQLine);

void IRQ_set_mask(uint8_t IRQLine);

#endif
