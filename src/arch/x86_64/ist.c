#include "ist.h"

TSSdesc new_TSSdesc() {
    TSSdesc t;

    t.zero = 0;
    t.zeros = 0;

    t.reserved_1 = 0;
    t.reserved_2 = 0;
    t.reserved_3 = 0;

    return t;
}
