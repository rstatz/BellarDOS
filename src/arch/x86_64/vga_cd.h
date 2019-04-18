#ifndef H_VGA_CD
#define H_VGA_CD

#define VGA_COLOR_BLACK 0x00
#define VGA_COLOR_BLUE 0x01
#define VGA_COLOR_GREEN 0x02
#define VGA_COLOR_CYAN 0x03
#define VGA_COLOR_RED 0x04
#define VGA_COLOR_MAGENTA 0x05
#define VGA_COLOR_BROWN 0x06
#define VGA_COLOR_LIGHT_GREY 0x07
#define VGA_COLOR_WHITE 0x0F

// Initializes Display
void VGA_init();

// Clears screen
void VGA_clear();

// Sets character colors
void VGA_set_color(char, char);

// Displays character
void VGA_display_char(unsigned char);

// Displays string
void VGA_display_str(unsigned char*);

#endif
