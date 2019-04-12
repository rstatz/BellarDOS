#include "math.h"
#include "print.h"

#define NEG_EX "\nmath.h: Negative Exponent not supported\n"

int exp(int x, int y) {
    int i, e = x;
    
    if (y == 0)
        return 1;

    if (y < 0) {
        print_str(NEG_EX);
        return 0;
    }

    for (i = 1; i < y; i++) {
        e = e * x;
    }

    return e;
}
