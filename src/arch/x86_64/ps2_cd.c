#include "ps2_cd.h"
#include "print.h"
#include "io.h"
#include "pic_cd.h"

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
#define IRQ_KEYBOARD_CFG 0x01
#define IRQ_MOUSE_CFG 0x02
#define TRANS_BIT 0x40
#define CLOCK_PORT1 0x10
#define CLOCK_PORT2 0x20
#define SELF_TEST_SUCCESS 0x55
#define SCANCODE_1 0x01
#define SCANCODE_2 0x02
#define PS2_ACK 0xFA
#define PS2_ECHO 0xEE
#define CONFIRM_SC_1 0x43
#define CONFIRM_RESET 0xAA
#define RESEND 0xFE
#define PS2_TWOBYTE 0xE0
#define PS2_TRIBYTE 0xE1

// Commands
#define KB_RESET 0xFF
#define READ_CONFIG 0x20
#define SET_CONFIG 0x60
#define SELF_TEST 0xAA
#define SET_SCANCODE 0xF0
#define ENABLE_KEYBOARD 0xF4
#define DISABLE_KEYBOARD 0xF5

#define CAPS_IN 0x3A
#define RSHIFT_IN 0x36
#define RSHIFT_OUT 0xB6
#define LSHIFT_IN 0x2A
#define LSHIFT_OUT 0xAA

#define IRQLINE_KEYBOARD 1

#define ASCII_BS 0x08

#define SHELL_PROMPT "> "

static uint8_t shift = 0;
static uint8_t caps = 0;
static uint8_t sc_s1[256] = {0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', ASCII_BS, '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /*numlock*/, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t sc_s1_caps[256] = {0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', ASCII_BS, '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /*numlock*/, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t sc_s1_shift[256] = {0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', ASCII_BS, '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /*numlock*/, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t sc_s1_shift_caps[256] = {0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', ASCII_BS, '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '\"', '~', 0, '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /*numlock*/, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static uint8_t* charset = sc_s1;

void ps2_poll_write(uint8_t c) {
    while (inb(PS2_CMD_REG) & ISTATUS_MASK);
    outb(PS2_CMD_REG, c);
}

void ps2_poll_write2(uint8_t cmd, uint8_t data) {
    while (inb(PS2_CMD_REG) & ISTATUS_MASK);
    outb(PS2_CMD_REG, cmd);
    while (inb(PS2_CMD_REG) & ISTATUS_MASK);
    outb(PS2_DATA_REG, data);
}

void ps2_poll_write_dev(uint8_t data) {
    while (inb(PS2_CMD_REG) & ISTATUS_MASK);
    outb(PS2_DATA_REG, data);
}

uint8_t ps2_poll_read() {
    uint8_t status = inb(PS2_CMD_REG);

    while (!(status & OSTATUS_MASK))
        status = inb(PS2_CMD_REG);

    return inb(PS2_DATA_REG);
}

void set_charset() {
    if (caps && shift)
        charset = sc_s1_shift_caps;
    else if (caps)
        charset = sc_s1_caps;
    else if (shift)
        charset = sc_s1_shift;
    else
        charset = sc_s1;
}

void ps2_read_keyboard(uint8_t c) {
    switch(c) {
        case(PS2_TWOBYTE) :
            ps2_poll_read();
            break;

        case(PS2_TRIBYTE) :
            ps2_poll_read();
            ps2_poll_read();
            break;

        case(CAPS_IN) :
            caps ^= 1;
            set_charset();
            break;
                
        case(RSHIFT_IN) :
            shift = 1;
            set_charset();
            break;

        case(LSHIFT_IN) :
            shift = 1;
            set_charset();
            break;
            
        case(RSHIFT_OUT) :
            shift = 0;
            set_charset();
            break;

        case(LSHIFT_OUT) :
            shift = 0;
            set_charset();
            break;

        default:
            if ((c = charset[c]) == 0)
                break;
                
            printk("%c", c);

            if (c == '\n' || c == '\r')
                printk(SHELL_PROMPT);

            break;
    }
}

void ps2_poll_keyboard() {
    uint8_t c;
    
    while (1) {
        c = ps2_poll_read();

        ps2_read_keyboard(c);
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
    uint8_t config;
    
    ps2_flush_out();

    ps2_poll_write(READ_CONFIG); 
    config = ps2_poll_read();
    
    //printk("ps2: Config=%x\n", config);
    
    config |= CLOCK_PORT2 | IRQ_KEYBOARD_CFG;
    config &= ~(IRQ_MOUSE_CFG | TRANS_BIT | CLOCK_PORT1); // Translation?
  
    //printk("ps2: Writing status %x\n", config);
 
    ps2_poll_write2(SET_CONFIG, config);
}

void ps2_poll_keyboard_reset() {
    uint8_t status;

    ps2_poll_write_dev(KB_RESET);    
    //printk("ps2: Keyboard Reset Sent...\n");
    
    if (!ps2_ack())
        printk("ps2: Keyboard reset failed\n");

    if ((status = ps2_poll_read()) == CONFIRM_RESET) {}
    //    printk("ps2: Keyboard reset\n");
    //else
    //    printk("ps2: Keyboard reset failed\n");
}

void ps2_set_scancode() {
    //printk("ps2: Setting Scancode\n");   

    ps2_poll_write_dev(SET_SCANCODE);
    if(!ps2_ack()) {}
        //printk("Error: Scan code not set\n");
    
    ps2_poll_write_dev(SCANCODE_1);
    if(!ps2_ack()) {}
        //printk("Error: Scan code not set\n");
}

void ps2_keyboard_init() {
    // Reset keyboard
    ps2_poll_keyboard_reset();
    
    // Set Scancode
    ps2_set_scancode();

    // Enable Keyboard
    ps2_poll_write_dev(ENABLE_KEYBOARD);
    //if (ps2_ack())
    //    printk("ps2: Keyboard Enabled\n");
}

void ps2_self_test() {
    ps2_poll_write(SELF_TEST);
    
    if (ps2_poll_read() != SELF_TEST_SUCCESS) {
        printk("Error: ps2 controller not configured properly\n");
        return;
    }

    //ps2_poll_write(0xAB);

    //if (ps2_poll_read() == 0x00)
    //    printk("ps2: Port 1 passes\n");
}

void ps2_init() {
    //printk("ps2: Initializing...\n");
    
    // Disable Ports
    ps2_poll_write(DISABLE_PORT1);
    //printk("ps2: Disabled port 1\n");
 
    ps2_poll_write(DISABLE_PORT2);    
    //printk("ps2: Disabled port 2\n");
    
    // test controller 
    ps2_self_test();

    ps2_poll_write(DISABLE_PORT1);
    ps2_poll_write(DISABLE_PORT2);

    // Set Config
    ps2_set_config();
    
    //printk("ps2: Configured Controller\n");

    ps2_poll_write(ENABLE_PORT1);
        
    //printk("ps2: Port 1 Enabled\n");

    ps2_keyboard_init();
    
    IRQ_clear_mask(IRQLINE_KEYBOARD);

    //ps2_poll_write(ENABLE_PORT2);
}

void IRQ_keyboard_handler() {
    uint8_t c;

    c = ps2_poll_read();
    ps2_read_keyboard(c);
}
