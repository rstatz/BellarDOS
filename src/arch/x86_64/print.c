#include <stdint.h>
#include <stdarg.h>
#include "print.h"
#include "vga_cd.h"
#include "math.h"

#define INT_MXDGTS 10
#define SHORT_MXDGTS 5
#define LONG_MXDGTS 8
#define QUAD_MXDGTS 20

#define SHORT_MXDGTS_HX 4
#define INT_MXDGTS_HX 8
#define LONG_MXDGTS_HX 8
#define QUAD_MXDGTS_HX 16
#define PTR_MXDGTS_HX 8

#define LOWER_NIB 0x0F

#define ASCII_0 0x30
#define ASCII_A 0x41

// %c
void print_char(char c) {
    VGA_display_char(c);
}

// %s
void print_str(const char* str) {
    VGA_display_str((const unsigned char*)str);
}

void print_uchar(unsigned char c) {
    VGA_display_char(c);
}

// %h %hd
void print_short(short s) {
    unsigned short mag = s;
    uint8_t c, i, z_ignore = 1;
 
    if (s < 0) {
        print_char('-');
        mag = -mag;
    }

    for (i = 1; i <= SHORT_MXDGTS; i++) {
        c = (i == SHORT_MXDGTS) ? (mag % 10) : 
                ((mag/(exp(10, (SHORT_MXDGTS - i)))) % 10); 
        c += ASCII_0;

        if ((c != '0') || (!z_ignore)) {
            print_char(c);
            z_ignore = 0;
        }
        else if (i == SHORT_MXDGTS)
            print_char(c);
    }
}

// %d %l %ld
void print_int(int d) {
    unsigned int mag = d;
    uint8_t c, i, z_ignore = 1;
 
    if (d < 0) {
        print_char('-');
        mag = -mag;
    }

    for (i = 1; i <= INT_MXDGTS; i++) {
        c = (i == INT_MXDGTS) ? (mag % 10) : 
                ((mag/(exp(10, (INT_MXDGTS - i)))) % 10); 
        c += ASCII_0;

        if ((c != '0') || (!z_ignore)) {
            print_char(c);
            z_ignore = 0;
        }
        else if (i == INT_MXDGTS)
            print_char(c);
    }
}

// %q %qd
void print_quad(long long q) {
    unsigned long long mag = q;
    uint8_t c, i, z_ignore = 1;
 
    if (q < 0) {
        print_char('-');
        mag = -mag;
    }

    for (i = 1; i <= QUAD_MXDGTS; i++) {
        c = (i == QUAD_MXDGTS) ? (mag % 10) : 
                ((mag/(exp(10, (QUAD_MXDGTS - i)))) % 10); 
        c += ASCII_0;

        if ((c != '0') || (!z_ignore)) {
            print_char(c);
            z_ignore = 0;
        }
        else if (i == QUAD_MXDGTS)
            print_char(c);
    }
}

void print_u(unsigned long d, int digits) {
    uint8_t c, i, z_ignore = 1;

    for (i = 1; i <= digits; i++) {
        c = (i == digits) ? (d % 10) : 
                ((d / (exp(10, (digits - i)))) % 10); 
        c += ASCII_0;

        if ((c != '0') || (!z_ignore)) {
            print_char(c);
            z_ignore = 0;
        }
        else if (i == (digits - 1))
            print_char(c);
    }
}

// %hu
void print_ushort(unsigned short s) {
    print_u(s, SHORT_MXDGTS);
}

// %u
void print_uint(unsigned int d) {
    print_u(d, INT_MXDGTS);
}

// %lu
void print_ulong(unsigned long l) {
    print_u(l, LONG_MXDGTS);
}

// %qu
void print_uquad(unsigned long long q) {
    print_u(q, QUAD_MXDGTS);
}

void print_x(long l, int digits) {
    int i;
    uint8_t c;

    print_str("0x");

    for (i = 1; i <= digits; i++) {
        c = (l >> ((digits - i) * 4)) & LOWER_NIB;
        c += (c < 10) ? (ASCII_0) : (ASCII_A - 10);

        print_char(c);
    }
}

// %hx
void print_shortx(short s) {
    print_x(s, SHORT_MXDGTS_HX);
}

// %x
void print_intx(int d) {
    print_x(d, INT_MXDGTS_HX);
}

// %lx
void print_longx(long l) {
    print_x(l, LONG_MXDGTS_HX);
}

// %qx
void print_quadx(long long q) {
    print_x(q, QUAD_MXDGTS_HX);
}

// %p
void print_pointer(void* p) {
    print_x((long)p, PTR_MXDGTS_HX);
}

char* parse_h(va_list args, char* s) {
    short h;
    
    switch(*(s + 1)) {
        case('d') : h = (short)va_arg(args, int);
                    print_short(h);
                    return ++s;

        case('u') : h = (short)va_arg(args, int);
                    print_ushort((unsigned short)h);
                    return ++s;

        case('x') : h = (short)va_arg(args, int);
                    print_shortx(h);
                    return ++s;

        default : h = (short)va_arg(args, int);
                  print_short(h);
                  return s;
    }
}

char* parse_l(va_list args, char* s) {
    long l;

    switch(*(s + 1)) {
        case('d') : l = va_arg(args, long);
                    print_int((int)l);
                    return ++s;

        case('u') : l = va_arg(args, long);
                    print_ulong((unsigned long)l);
                    return ++s;

        case('x') : l = va_arg(args, long);
                    print_longx(l);
                    return ++s;

        default : l = va_arg(args, long);
                  print_int((int)l);
                  return s;
    }
}

char* parse_q(va_list args, char* s) {
    long long q;

    switch(*(s + 1)) {
        case('d') : q = va_arg(args, long long);
                    print_quad(q);
                    return ++s;

        case('u') : q = va_arg(args, long long);
                    print_uquad((unsigned long long)q);
                    return ++s;

        case('x') : q = va_arg(args, long long);
                    print_quadx(q);
                    return ++s;

        default : q = va_arg(args, long long);
                  print_int(q);
                  return s;
    }
}

int printk(const char* fmt, ...) {
    char* s = (char*)fmt;
    long long d;
    va_list args;
    va_start(args, fmt);
    
    while (*s != '\0') {
        if (*s == '%') {
            s++;
            
            switch(*s) {
                case('%') : print_char('%');
                            break;
                
                case('d') : d = va_arg(args, int);
                            print_int((int)d);
                            break;
                
                case('u') : d = va_arg(args, unsigned int);
                            print_uint((unsigned int)d);
                            break;

                case('x') : d = va_arg(args, int);
                            print_intx((int)d);
                            break;
    
                case('c') : d = (long long)va_arg(args, int);
                            print_char((char)d);
                            break;

                case('p') : d = (long long)va_arg(args, void*);
                            print_pointer((void*)d);
                            break;

                case('s') : d = (long long)va_arg(args, char*);
                            print_str((char*)d);
                            break;

                case('h') : s = parse_h(args, s);
                            break;

                case('l') : s = parse_l(args, s);
                            break;

                case('q') : s = parse_q(args, s);
                            break;

                default: print_str("\nError: bad format specifier\n");
                         return -1;
            }
        }
        else
            print_char(*s);
        
        s++;
    }

    va_end(args);

    return 0;
}

