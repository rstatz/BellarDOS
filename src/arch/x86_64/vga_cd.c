#include <stdbool.h>
#include <stddef.h>

#include "strings.h"
#include "vga_cd.h"
#include "irq.h"
#include "debug.h"

#define VGA_DEFAULT_WIDTH 80
#define VGA_DEFAULT_HEIGHT 25

#define VGA_BASE 0xb8000

#define VGA_ENTRY(H, C) ((H << 8) | C)

#define ASCII_BS 0x08

static int width = VGA_DEFAULT_WIDTH;
static int height = VGA_DEFAULT_HEIGHT;

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
    cursor = 0;
}

void VGA_init(char fg, char bg) {
    VGA_set_color(fg, bg);

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

void VGA_display_char(unsigned char c) {
    // Different support for carriage return?
    uint8_t disabled_ints = 0;

    if (irq_enabled()) {
        disabled_ints = 1;
        CLI;
    }
    
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

    if (disabled_ints)
        STI;
}

void VGA_display_str(unsigned char* str) {
    unsigned char* p = str;

    while (*p != '\0') {
        VGA_display_char(*p);
        p++;
    }
}

int VGA_row_count() {
    return height;
}

int VGA_col_count() {
    return width;
}

void VGA_dislay_attr_char(int x, int y, char c, int fg, int bg) {
    VGA_set_color(fg, bg);

    cursor = (y * width) + x;

    VGA_display_char((unsigned char) c);
}

