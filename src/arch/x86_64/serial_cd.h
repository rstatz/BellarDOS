#ifndef SERIAL_CD_H
#define SERIAL_CD_H

#define SERIAL_BUFF_SIZE 16

typedef struct serial_buff {
    char buff[SERIAL_BUFF_SIZE];
    char *head, *tail, *next;
} serial_buff;

void SER_init();

int SER_tx(unsigned char);

int SER_write(char);

void SER_write_str(const char*);

void IRQ_SER_tx();

#endif
