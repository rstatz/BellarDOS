#include "splash.h"
#include "print.h"
#include "vga_cd.h"

#define SHELL_PROMPT "> "
#define WELCOME "*Welcome to BellarDOS*\n"

void splash() {
    char* img = "                                oooo     oooo\n\
                              o$$\"\"\"$oo$$\"\"\"\"\"$o\n\
                             $\"      $$\"      $$$$\n\
                            $\"      \"\"        $$$$$o\n\
                            $                  $$$$$o\n\
                           $                    $$$$$$\n\
                          $\"                    \"$$$$$\n\
                          $                      $$$$$$\n\
                         $\"                      $$$$$$\n\
                         $                        $$$$$\n\
                         $        BellarDOS      o$$$$$\n\
                         $                       $$$$$$\n\
                        $                      o$$$$$$\n\
                      ooo                      o$$$$$$$\n\
              ooo$$$$\"\" $                   oo$$$$$\"\"\"\"\"\"oooo\n\
           oo\"$$$$$$$ oo\"\" oooooooooooooooo$$\"\"\"           o$$\"oo\n\
          o\"  $$$$$$$ \"$o           oo$$$$$\"               $$$$o\"$o\n\
         $    $$$$$$$  \" \"\"oooooooooo$$$$\"         o$      $$$$$$o\"$\n\
        o     $$\"\"               oo$$$\"           o$$     o$$$$$$$o$\n\
        \"o    $$             oo$$$$\"\"            o$$$   o$$$$$$$$$$$\n\
         \"$o  $$$oo                           $$$$$$$   ooo$$$$$\"\"\n\
           \"$$oooo \"\"            ooo$$$$      $$$$$$$$$$$$$$\"\"\n\
               \"\"\"\"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\"\"\"\"\"\"\n\
                         \"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\n\
                                   v0.0.0.1\n";

    VGA_init(VGA_WHITE, VGA_WHITE);
    VGA_set_color(VGA_BLACK, VGA_WHITE);
    //VGA_clear();
    VGA_display_str((unsigned char*)img);
}

void splash_end() {
    VGA_set_color(VGA_CYAN, VGA_ORANGE);
    VGA_clear();

    printk(WELCOME);
    printk(SHELL_PROMPT);
}
