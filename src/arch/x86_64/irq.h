#include <stdint.h>

#ifndef IRQ_H
#define IRQ_H

#define CLI asm("CLI")
#define STI asm("STI")

void gen_gpf();

static inline uint16_t irq_enabled() {
    uint16_t flags;

    asm volatile("pushfq\n\t"
                 "pop %0\n\t" 
                 : "=rm" (flags) : : "memory");

    // Returns flag register masked with the interrupt flag position
    return flags & 0x200;
}

#endif
