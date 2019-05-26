#include <stdint.h>
#include "mmap.h"
#include "print.h"
#include "debug.h"

#define TAG_TERM 0
#define TAG_BOOT_CMD 1
#define TAG_BOOT_LOAD 2
#define TAG_BASIC_MEM 4
#define TAG_BIOS_BOOT_DEV 5
#define TAG_MEM_MAP 6

void parse_tags(void* tag) {
    uint32_t type, size;
    char* str;

    tag += sizeof(MB_tag_header);
    
    while ((type = ((MB_tag*)tag)->tag_type) != TAG_TERM) {
//        printk("Tag #%d, Size = %d\n", type, ((MB_tag*)tag)->tag_size);

        switch (type) {
            case(TAG_BOOT_CMD) :
                break;
            case(TAG_BOOT_LOAD) :
                str = (char*)(&((MB_tag_boot_loader*)tag)->bootloader_name);
                printk("Bootloader: %s\n", str);
                break;
            case(TAG_MEM_MAP) :
                break;
            default:
                break;
        }
        
        size = ((MB_tag*)tag)->tag_size;
        size += 8 - (size % 8); // Keeps 8-byte aligned
        tag += size;
    }
}

void mmap(PML4_ref pml, void* tag) {
    printk("Tag address: %p\n", tag);
    printk("Header size: %d\n", ((MB_tag_header*)tag)->tag_size);
    parse_tags(tag);
}
