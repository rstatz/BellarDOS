#ifndef PIC_CD_H
#define PIC_CD_H

#include <stdint.h>

#define CLI asm("CLI")
#define STI asm("STI")

void pic_init();

void PIC_sendEOI(uint8_t);

#endif
