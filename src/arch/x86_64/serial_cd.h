#ifndef SERIAL_CD_H
#define SERIAL_CD_H

#define SERIAL_BUFF_SIZE 16

typedef struct serial_buff {
    char buff[SERIAL_BUFF_SIZE];
    char *head, *tail;
} serial_buff;

void SER_init();

int SER_write(const char*, int);

#endif
