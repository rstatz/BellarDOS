#ifndef H_VGA_CD
#define H_VGA_CD

#define VGA_BLACK 0x00
#define VGA_BLUE 0x01
#define VGA_GREEN 0x02
#define VGA_CYAN 0x03
#define VGA_RED 0x04
#define VGA_PURPLE 0x05
#define VGA_ORANGE 0x06
#define VGA_LIGHT_GREY 0x07
#define VGA_DARK_GREY 0x08
#define VGA_BRIGHT_BLUE 0x09
#define VGA_BRIGHT_GREEN 0x0A
#define VGA_BRIGHT_CYAN 0x0B
#define VGA_MAGENTA 0x0C
#define VGA_BRIGHT_PURPLE 0x0D
#define VGA_YELLOW 0x0E
#define VGA_WHITE 0x0F

// Initializes Display
void VGA_init(char, char);

// Clears screen
void VGA_clear();

// Sets character colors
void VGA_set_color(char, char);

// Displays character
void VGA_display_char(unsigned char);

// Displays string
void VGA_display_str(unsigned char*);

//Snakes
int VGA_row_count();
int VGA_col_count();
void VGA_clear();
void VGA_display_attr_char(int, int, char, int, int);

#endif
