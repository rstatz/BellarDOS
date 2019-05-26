#include <stdint.h>
#include <stddef.h>
#include "mmap.h"
#include "print.h"
#include "debug.h"

#define TAG_TERM 0
#define TAG_BOOT_CMD 1
#define TAG_BOOT_LOAD 2
#define TAG_BASIC_MEM 4
#define TAG_BIOS_BOOT_DEV 5
#define TAG_MEM_MAP 6
#define TAG_ELF_SYMBOLS 9

#define MMAP_TAG_SIZE 16
#define ELF_SYMBL_TAG_SIZE 20

void parse_tag_mmap(void* mmap_tag) {
    int i, num_info;    
    MB_tag_mmap mem_header;
    MB_mem_info* mem;

    if (mmap_tag == NULL) {
        printk("Multiboot memory info tag not found\n");
        return;
    }

    mem_header = *(MB_tag_mmap*)mmap_tag;
    mem = (MB_mem_info*)(&(mem_header.mem_info_entries));
    num_info = (mem_header.tag_size - MMAP_TAG_SIZE) / mem_header.mem_info_size;
//    BREAK;
    for (i = 0; i < num_info; i++) {
        printk("Mem Region Type %d\n", mem[i].type);
        if (mem[i].type == 1) {
            printk("Available Mem Region Found at %p\n", (void*)mem[i].start_addr);
        }
    }
}

void parse_tag_elf_symbols(void* elf_tag) {
    int i, num_sects;
    MB_tag_elf_symbols elf_header;
    MB_elf_sect_header* sects;
    char *name, *str_table;

    if (elf_tag == NULL) {
        printk("Multiboot elf symbols tag not found\n");
        return;
    }

    elf_header = *(MB_tag_elf_symbols*)elf_tag;
    sects = (MB_elf_sect_header*)(&(elf_header.sect_head_array));
    num_sects = (elf_header.tag_size - ELF_SYMBL_TAG_SIZE) / elf_header.sect_head_size;
    str_table = (char*)elf_header.index_str_table_sect;

    for (i = 0; i < num_sects; i++) {
        name = &str_table[sects[i].sect_name_index];
        printk("Found ELF sect: %s\n", name);
    }
}

void parse_tag_bootloader(void* tag) {
    char* str;
    
    if (tag == NULL) {
        printk("Multiboot bootloader tag not found\n");
        return;
    }

    str = (char*)(&((MB_tag_bootloader*)tag)->bootloader_name);
    printk("Bootloader: %s\n", str);
}

void parse_mb_tags(void* tag) {
    void* mem = NULL;
    void* bootl = NULL;
    void* elf = NULL;
    uint32_t type, size;

    tag += sizeof(MB_tag_header);
    
    while (!(((type = ((MB_tag*)tag)->tag_type) == TAG_TERM) 
            && (((MB_tag*)tag)->tag_size == 8))) {
        printk("Tag #%d, Size = %d\n", type, ((MB_tag*)tag)->tag_size);

        switch (type) {
            case(TAG_BOOT_LOAD) :
                bootl = tag;
                break;
            case(TAG_MEM_MAP) :
                mem = tag;
                break;
            case(TAG_ELF_SYMBOLS) :
                elf = tag;
                break;
            default:
                break;
        }
        
        size = ((MB_tag*)tag)->tag_size;
        size = (size % 8) ? size + 8 - (size % 8) : size; // Keeps 8-byte aligned
//        size = (type == 16) ? size + 8 : size;
        tag += size;
    }

    parse_tag_bootloader(bootl);
    parse_tag_elf_symbols(elf);
    parse_tag_mmap(mem);
}

void MMU_setup(PML4_ref pml, void* tag) {
    printk("Tag address: %p\n", tag);
    printk("Header size: %d\n", ((MB_tag_header*)tag)->tag_size);
    parse_mb_tags(tag);
}
