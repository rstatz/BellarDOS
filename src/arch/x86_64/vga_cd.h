#ifndef H_VGA_CD
#define H_VGA_CD

// Initializes Display
void VGA_init();

// Clears screen
void VGA_clear();

// Displays character
void VGA_display_char(char);

// Displays string
void VGA_display_str(const unsigned char*);

#endif
