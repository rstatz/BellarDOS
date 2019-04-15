unsigned char inb(unsigned int port) {
    return *((char*)(long)port);
}

void outb(unsigned char value, unsigned int port) {
    *((char*)(long)port) = value;
}
