#include "strings.h"

#include "gdt.h"
#include "idt.h"
#include "irq.h"

#include "pic_cd.h"
#include "vga_cd.h"
#include "ps2_cd.h"
#include "print.h"

#include "splash.h"

#include "debug.h"

#define PULSE_DELAY 100000000
#define SPLASH_DELAY 300000000

void delay_cycles(unsigned int i) {
    unsigned int c = 0;

    while (c != i)
        c++;
}

void kmain() {    

    CLI; 

    VGA_init(VGA_COLOR_WHITE, VGA_COLOR_WHITE);
 
    splash();

    pic_init();    
    idt_init();
    idt_load(255);

//    BREAK;

    gdt_init();
    gdt_load();   

//    BREAK;

    ps2_init();

    delay_cycles(SPLASH_DELAY);

    splash_end();

    BREAK;

    STI;

    while(1) {
//        delay_cycles(PULSE_DELAY);
//        printk(".");
//        ps2_poll_keyboard();
        asm("hlt");
    }
}
