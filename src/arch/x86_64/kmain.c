#include "strings.h"
#include "vga_cd.h"
#include "print.h"

void kmain() {    

    VGA_init();

    VGA_display_char('W');
    VGA_display_char('O');
    VGA_display_char('R');
    VGA_display_char('K');

    VGA_display_str((unsigned char*)"\nHello world\n");
    VGA_display_str((unsigned char*)"TESTING\n\n\n\n\n\n\n\n"); 

    print_short(-31040);
    print_char('\n');
    print_int(4000000000);
    print_char('\n');
    print_uint(4000000000);
    print_char('\n');
    print_longx(0x89abcdef);

    VGA_display_str((unsigned char*)"\n\n\n\n");
    VGA_display_str((unsigned char*)"DOWN HERE\n HEYO\n\n\n\n\n\nLAST\nSCROLL");

    while(1) {
        asm("hlt");
    }
}
