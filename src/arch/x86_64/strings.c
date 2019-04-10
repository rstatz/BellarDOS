#include "strings.h"

void* memset(void* dest, int c, size_t n) {
    int i;

    for (i = 0; i < n; i++) {
        ((char*)dest)[i] = c;
    }

    return dest;
}

void* memcpy(void* dest, const void* src, size_t n) {
    int i;

    for (i = 0; i < n; i++) {
        ((char*)dest)[i] = ((char*)src)[i];
    }

    return dest;
}

size_t strlen(const char* s) {
    size_t c = 0;

    while(s[0] != '\0') {
        c++;
    }
    
    return c;
}

char* strcpy(char* dest, const char* src) {
    int c = 0;

    while (src[c] != '\0') {
        dest[c] = src[c];
        c++;
    }

    dest[c] = '\0';

    return dest;
}

int strcmp(const char* s1, const char* s2) {
    int c = 0;

    while(s1[c]) {
        if (s1[c] != s2[c])
            break;
        
        c++;
    }

    return ((unsigned char*)s1)[c] - ((unsigned char *)s2)[c];
}

const char* strchr(const char* s, int c) {
    int c = 0;

    while (s[c] != '\0') {
        if (s[c] == c)
            return &s[c];
        c++;
    }

    return NULL;
}

char* strdup(const char* s);
