#include <stddef.h>
#include "strings.h"

void* memset(void* dest, int c, size_t n) {
    size_t i;

    for (i = 0; i < n; i++) {
        ((char*)dest)[i] = c;
    }

    return dest;
}

void* memcpy(void* dest, const void* src, size_t n) {
    size_t i;

    for (i = 0; i < n; i++) {
        ((char*)dest)[i] = ((char*)src)[i];
    }

    return dest;
}

size_t strlen(const char* s) {
    size_t i = 0;

    while(s[0] != '\0') {
        i++;
    }
    
    return i;
}

char* strcpy(char* dest, const char* src) {
    int i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';

    return dest;
}

int strcmp(const char* s1, const char* s2) {
    int i = 0;

    while(s1[i]) {
        if (s1[i] != s2[i])
            break;
        
        i++;
    }

    return ((unsigned char*)s1)[i] - ((unsigned char *)s2)[i];
}

const char* strchr(const char* s, int c) {
    int i = 0;

    while (s[i] != '\0') {
        if (s[i] == c)
            return &s[i];
        i++;
    }

    return NULL;
}

char* strdup(const char* s);
