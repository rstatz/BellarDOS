#include "multitask.h"

#ifndef PS2_CD_H
#define PS2_CD_H

#define DEFAULT_KBD_BUF_SIZE 100

typedef struct KBD_q {
    Proc_q blocked;

    char buf[DEFAULT_KBD_BUF_SIZE];
    
    char *tail, *read, *write;
} KBD_q;

void IRQ_keyboard_handler();
void ps2_poll_keyboard();
void ps2_init();

#endif
