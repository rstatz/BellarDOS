#include "ps2_cd.h"
#include "print.h"

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
#define ISTATUS (ISTATUS_MASK & *ps2_cmd)
#define OSTATUS_MASK 0x01
#define OSTATUS (OSTATUS_MASK & *ps2_cmd)

#define IRQ_BITS 0x03
#define TRANS_BIT 0x40
#define CLOCK_PORT1 0x10
#define CLOCK_PORT2 0x20
#define SELF_TEST_SUCCESS 0x55
#define SCANCODE_1 0x01
#define CONFIRM_SC_1 0x43
#define CONFIRM_RESET 0xAA

// Commands
#define KB_RESET 0xFF
#define READ_CONFIG 0x20
#define SET_CONFIG 0x60
#define SELF_TEST 0xAA
#define SET_SCANCODE 0xF0
#define ENABLE_KEYBOARD 0xF4
#define DISABLE_KEYBOARD 0xF5

static char* ps2_cmd = (char*)PS2_CMD_REG;
static char* ps2_data = (char*)PS2_DATA_REG;

void ps2_poll_write(char c) {
    while (!ISTATUS);
    *ps2_cmd = c;
}

void ps2_poll_write2(char c1, char c2) {
    while (!ISTATUS);
    *ps2_cmd = c1;
    *ps2_data = c2; 
}

char ps2_poll_read() {
    while (!OSTATUS);
    return *ps2_data;
}

void ps2_poll_keyboard() {
    while (1) {
        if (ps2_poll_read() == (char)0x13) {
            if (ps2_poll_read() == (char)0x93)
                printk("R");
        }
    } 
}

void ps2_flush_out() {
    
}

void ps2_keyboard_init() {
    // Reset keyboard
    ps2_poll_write(KB_RESET);
    if (ps2_poll_read() != (char)CONFIRM_RESET)
        printk("Error: Keyboard not reset\n");

    // Set Scancode
    ps2_poll_write(SET_SCANCODE);
    ps2_poll_write(SCANCODE_1);
    if (ps2_poll_read() != CONFIRM_SC_1)
        printk("Error: Scan code not set\n");

    // Enable Keyboard
    ps2_poll_write(ENABLE_KEYBOARD);
}

void ps2_init() {
    char status;
    printk("ps2: Initializing...");
    // Disable Ports
    ps2_poll_write(DISABLE_PORT1);
    ps2_poll_write(DISABLE_PORT2);
    
    printk("ps2: Disabled ports\n");
    //ps2_flush_out();

    // Set Config
    ps2_poll_write(READ_CONFIG); 
    status = ps2_poll_read();
    printk("ps2: Status=%x\n", status);
    status &= ~(IRQ_BITS || TRANS_BIT);
    status |= CLOCK_PORT1;
    ps2_poll_write2(SET_CONFIG, status);

    // Self Test
    ps2_poll_write(SELF_TEST);
    if (ps2_poll_read() != SELF_TEST_SUCCESS) {
        printk("Error: ps2 controller not configured properly\n");
        return;
    }

    ps2_keyboard_init();

    ps2_poll_write(ENABLE_PORT1);
    //ps2_poll_write(ENABLE_PORT2);

    ps2_poll_keyboard();
}
