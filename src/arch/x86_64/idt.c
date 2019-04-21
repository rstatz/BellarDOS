#include <stdint.h>

#include "idt.h"

#define MASK_32 0x00000000FFFFFFFF
#define MASK_16 0x0000FFFF

IDTEntry IDTEntry(void* isr) {
    IDTEntry e;
   
    e.targ_offset_32 = (isr >> 32) & MASK_32;
    e.targ_offset2_16 = (isr >> 16) & MASK_16;
    e.targ_offset_16 = isr & MASK_16;

    e.zero = 0;

    return e;
}
