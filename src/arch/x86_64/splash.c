#include "vga_cd.h"

void splash() {
    char* img = "                        oooo     oooo\n\
                      o$$\"\"\"$oo$$\"\"\"\"\"$o\n\
                     $\"      $$\"      $$$$\n\
                    $\"      \"\"        $$$$$o\n\
                    $                  $$$$$o\n\
                   $                    $$$$$$\n\
                  $\"                    \"$$$$$\n\
                  $                      $$$$$$\n\
                 $\"                      $$$$$$\n\
                 $                        $$$$$\n\
                 $         BellarDOS     o$$$$$\n\
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

    VGA_set_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    VGA_clear();
    VGA_display_str((unsigned char*)img);
}
