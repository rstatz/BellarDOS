#include "strings.h"
#include "vga_cd.h"
#include "ps2_cd.h"
#include "print.h"
#include "splash.h"

#define SPLASH_DELAY 300000000

void delay_cycles(unsigned int i) {
    unsigned int c = 0;

    while (c != i)
        c++;
}

void kmain() {    

    VGA_init(VGA_COLOR_WHITE, VGA_COLOR_WHITE);

//    VGA_display_char('W');
//    VGA_display_char('O');
//    VGA_display_char('R');
//    VGA_display_char('K');

    //VGA_display_str((unsigned char*)"\nHello world\n");
    //VGA_display_str((unsigned char*)"TESTING\n\n\n\n\n\n\n\n"); 

    //print_short(-31040);
    //print_char('\n');
    //print_int(4000000000);
    //print_char('\n');
    //print_uint(4000000000);
    //print_char('\n');
    //printk("%x", 0xBB);

    //printk("\n Testing %s with %d %crgs, which is %hx in hex %qx", "printk", 5, 'a', (short)5, (unsigned long long)8446744073709551615);

    //VGA_display_str((unsigned char*)"\n\n\n");
    //VGA_display_str((unsigned char*)"DOWN HERE\n HEYO\n\n\n\n\n\nLAST\nSCROLL\n");

    splash();

    ps2_init();
    
    delay_cycles(SPLASH_DELAY);

    splash_end();

    while(1) {
        ps2_poll_keyboard();
        //asm("hlt");
    }
}
