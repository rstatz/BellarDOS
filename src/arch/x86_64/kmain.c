#include "strings.h"

#include "gdt.h"
#include "idt.h"
#include "irq.h"

#include "pic_cd.h"
#include "vga_cd.h"
#include "ps2_cd.h"
#include "serial_cd.h"
#include "print.h"
#include "irq.h"

#include "splash.h"

#include "debug.h"

#define PULSE_DELAY 100000000
#define SPLASH_DELAY 200000000

void delay_cycles(unsigned int i) {
    unsigned int c = 0;

    while (c != i)
        c++;
}

void kmain() {    
    CLI; 

    // x86_64 Setup
    gdt_init();
    gdt_load();
    idt_init();
    idt_load();

    // Splash and VGA init
    splash();   

    // Device Initialization
    pic_init();
    ps2_init();
    SER_init();

    // Splash End
    delay_cycles(SPLASH_DELAY);
    splash_end();

//    BREAK;

    STI;

//    gen_gpf();

    while(1) {
        asm("hlt");
    }
}
