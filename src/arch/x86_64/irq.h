#include <stdint.h>

#ifndef IRQ_H
#define IRQ_H

#define CLI asm("CLI")
#define STI asm("STI")

static inline uint16_t irq_enabled() {
    uint16_t flags;

    asm volatile("pushf : pop %0" : "=rm" (flags) : : "memory");

    // Returns flag register masked with the interrupt flag position
    return flags & 0x200;
}

#endif
