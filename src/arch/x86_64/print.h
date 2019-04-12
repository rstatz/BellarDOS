#ifndef PRINT_H
#define PRINT_H

void print_char(char);

void print_uchar(unsigned char);

void print_str(const char*);

void print_short(short);

void print_ushort(unsigned short);

void print_int(int);

void print_uint(unsigned int);

void print_longx(long);

void print_pointer(void*);

// Must support: %% %d %u %x %c %p %h[dux] %l[dux] %q[dux] %s
void printk(const char*, ...);

#endif
