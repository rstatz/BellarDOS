#include "debug.h"

void bpoint() {
    int i = 1;

    while(i);
}

void halt_cpu() {
    asm volatile ("hlt");
}
