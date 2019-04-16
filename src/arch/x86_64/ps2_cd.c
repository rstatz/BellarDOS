#include "ps2_cd.h"
#include "print.h"
#include "io.h"

// Ports
#define PS2_CMD_REG 0x64
#define PS2_DATA_REG 0x60

// Keyboard
#define ENABLE_PORT1 0xAE
#define DISABLE_PORT1 0xAD

// Mouse
#define ENABLE_PORT2 0xA8
#define DISABLE_PORT2 0xA7

// Status
#define ISTATUS_MASK 0x02
#define OSTATUS_MASK 0x01

#define IRQ_BITS 0x03
#define TRANS_BIT 0x40
#define CLOCK_PORT1 0x10
#define CLOCK_PORT2 0x20
#define SELF_TEST_SUCCESS 0x55
#define SCANCODE_1 0x01
#define PS2_ACK 0xFA
#define PS2_ECHO 0xEE
#define CONFIRM_SC_1 0x43
#define CONFIRM_RESET 0xAA
#define RESEND 0xFE

// Commands
#define KB_RESET 0xFF
#define READ_CONFIG 0x20
#define SET_CONFIG 0x60
#define SELF_TEST 0xAA
#define SET_SCANCODE 0xF0
#define ENABLE_KEYBOARD 0xF4
#define DISABLE_KEYBOARD 0xF5

static uint8_t sc_s2[132] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\t', '`', 0, 0, 0, 0, 0, 'q', '1', 0, 0, 0, 'z', 's', 'a', 'w', '2', 0, 0, 'c', 'x', 'd', 'e', '4', '3', 0, 0, ' ', 'v', 'f', 't', 'r', '5', 0, 0, 'n', 'b', 'h', 'g', 'y', '6', 0, 0, 0, 'm', 'j', 'u', '7', '8', 0, 0, ',', 'k', 'i', 'o', '0', '9', 0, 0, '.', '/', 'l', ';', 'p', '-', 0, 0, 0, '\'', 0, '[', '=', 0, 0, 0 /*caps*/, 0, '\n', ']', 0, '\\', 0, 0, 0, 0, 0, 0, 0, 0,0x08, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void ps2_poll_write(uint8_t c) {
    while (inb(PS2_CMD_REG) & ISTATUS_MASK);
    outb(c, PS2_CMD_REG);
}

void ps2_poll_write2(uint8_t cmd, uint8_t data) {
    while (inb(PS2_CMD_REG) & ISTATUS_MASK);
    outb(cmd, PS2_CMD_REG);
    outb(data, PS2_DATA_REG);
}

uint8_t ps2_poll_read() {
    uint8_t status = inb(PS2_CMD_REG);

    while (!(status & OSTATUS_MASK))
        status = inb(PS2_CMD_REG);

    return inb(PS2_DATA_REG);
}

void ps2_poll_keyboard() {
    uint8_t c;

    while (1) {
        c = ps2_poll_read();
        printk("%c", sc_s2[c]);
    } 
}

int ps2_ack() {
    if (ps2_poll_read() == PS2_ACK)
        return 1;
    return 0;
}

void ps2_flush_out() {
    inb(PS2_DATA_REG);
}

void ps2_set_config() {
    uint8_t status;

    ps2_poll_write(READ_CONFIG); 
    status = ps2_poll_read();
    
    printk("ps2: Status=%x\n", status);
    
    status &= ~(IRQ_BITS | TRANS_BIT | CLOCK_PORT1); // Translation?
   
    if (status & 0x20)
        printk("ps2: Two Channel device confirmed\n");
 
    ps2_poll_write2(SET_CONFIG, status);
}

void ps2_poll_keyboard_reset() {
    uint8_t status;

    ps2_poll_write(KB_RESET);    
    printk("ps2: Keyboard Reset Sent...\n");
    
    if ((status = ps2_poll_read()) == CONFIRM_RESET) {
        printk("ps2: Keyboard reset\n");
    }
    else if (status == 0xFC || status == 0xFD)
        printk("ps2: Keyboard reset failed\n");
    else if (status == RESEND)
        printk("RESEND?\n");
    //if (!ps2_ack())
    //    printk("ps2: Keyboard reset failed\n");
}

void ps2_set_scancode() {
    printk("ps2: Setting Scancode\n");
    
    //while(1);   

    ps2_poll_write(SET_SCANCODE);
    if(!ps2_ack())
        printk("Error: Scan code not set\n");
    
    ps2_poll_write(SCANCODE_1);
    if (ps2_poll_read() != CONFIRM_SC_1)
        printk("Error: Scan code not set\n");
    else
        printk("ps2: Scan code set\n");
}

void ps2_keyboard_init() {
    ps2_poll_write(DISABLE_KEYBOARD);
    if(!ps2_ack())
        printk("Error: Keyboard not Disabled\n");
 
    // Reset keyboard
    ps2_poll_keyboard_reset();
    
    // Set Scancode
    ps2_set_scancode();

    // Enable Keyboard
    ps2_poll_write(ENABLE_KEYBOARD);
    if (ps2_ack())
        printk("ps2: Keyboard Enabled\n");
}

void ps2_self_test() {
    ps2_poll_write(SELF_TEST);
    
    if (ps2_poll_read() != SELF_TEST_SUCCESS) {
        printk("Error: ps2 controller not configured properly\n");
        return;
    }

    printk("ps2: Configured Controller\n");

    ps2_poll_write(0xAB);

    if (ps2_poll_read() == 0x00)
        printk("ps2: Port 1 passes\n");
}

void ps2_init() {
    printk("ps2: Initializing...\n");
    
    // Disable Ports
    ps2_poll_write(DISABLE_PORT1);
    printk("ps2: Disabled port 1\n");
 
    ps2_poll_write(DISABLE_PORT2);    
    printk("ps2: Disabled port 2\n");
    
    ps2_flush_out();

    // Set Config
    ps2_set_config();

    // Self Test
    ps2_self_test();
    
    ps2_keyboard_init();

    ps2_poll_write(ENABLE_PORT1);

    //ps2_poll_write(ENABLE_PORT2);
}

