#include <stdbool.h>
#include <stddef.h>

#include "strings.h"

#define VGA_BASE 0xb8000

#define VGA_COLOR_BLACK 0x00
#define VGA_COLOR_BLUE 0x01
#define VGA_COLOR_GREEN 0x02
#define VGA_COLOR_CYAN 0x03
#define VGA_COLOR_RED 0x04
#define VGA_COLOR_MAGENTA 0x05
#define VGA_COLOR_BROWN 0x06
#define VGA_COLOR_LIGHT_GREY 0x07

#define VGA_ENTRY(H, C) ((H << 8) | C)

#define ASCII_BS 0x08

static int width = 80;
static int height = 25;

static int cursor = 0;
static unsigned char color = 0;

static unsigned short* vgaBuff = (unsigned short*)VGA_BASE;

void VGA_set_color(char fg, char bg) {
    color = fg | (bg << 4);
}

void VGA_clear() {
    int r, c;
    int block;

    for (r = 0; r < height; r++) {
        for (c = 0; c < width; c++) {
            block = (r * width) + c;
            vgaBuff[block] = VGA_ENTRY(color, (unsigned char)' ');
        }
    }
}

void VGA_init() {
    VGA_set_color(VGA_COLOR_CYAN, VGA_COLOR_RED);

    VGA_clear();
}

void scroll() {
    int r, c;
    int block, next_line;

    for (r = 0; r < height - 1; r++) {
        for (c = 0; c < width; c++) {
            block = (r * width) + c;
            next_line = block + width;
            vgaBuff[block] = vgaBuff[next_line];
        }
    }

    block = (width * (height - 1));

    for (c = 0; c < width; c++) {
        vgaBuff[block] = VGA_ENTRY(color, (unsigned char)' ');
        block++;
    }

    cursor -= width;
}

void VGA_display_char(char c) {
    // Different support for carriage return?
    if (cursor >= width*height)
        scroll();
    
    if ((c == '\n') | (c == '\r'))
        cursor = ((cursor / width) + 1) * width;
    else if (c == ASCII_BS) {
        cursor--;
        vgaBuff[cursor] = VGA_ENTRY(color, ' ');
    }
    else {
        vgaBuff[cursor] = VGA_ENTRY(color, c);
        cursor++;
    }
}

void VGA_display_str(char* str) {
    char* p = str;

    while (*p != '\0') {
        VGA_display_char(*p);
        p++;
    }
}

