#include "time.h"

void delay_cycles(unsigned int i) {
    unsigned int c = 0;

    while (c != i)
        c++;
}
