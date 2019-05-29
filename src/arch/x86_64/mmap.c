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

#define PG_SIZE 4096
#define DFLT_NUM_PGS 100

mem_map mmap;

void find_avl_mem(mem_map* mmap, mem_region r) {
    uint64_t ubase, uend, base, end;
    uint32_t i;
    mem_region temp, *new;

    base = r.base_addr;
    end = base + r.length - 1;

    for (i = 0; i < mmap->num_unavl; i++) {
        ubase = mmap->mem_unavl[i].base_addr;
        uend = ubase + mmap->mem_unavl[i].length - 1;
        
        if ((base >= ubase) && (end <= uend))
            return;

        if ((base < ubase) && (end > uend)) {
            temp.base_addr = base;
            temp.length = ubase - base;
            find_avl_mem(mmap, temp);

            temp.base_addr = uend + 1;
            temp.length = end - uend;
            find_avl_mem(mmap, temp);

            return;
        }

        if ((base <= uend) && (end > uend))
            base = uend + 1;

        if ((end >= ubase) && (base < ubase))
            end = ubase - 1;
    }
    
    printk("Available memory from %p to %p\n", (void*)base, (void*)end);

    new = &mmap->mem_avl[mmap->num_avl];
    new->base_addr = base;
    new->length = end - base + 1;
    mmap->num_avl++;
}

void parse_tag_mmap(mem_map* mmap, void* mmap_tag) {
    uint32_t i, num_info;    
    MB_tag_mmap* mem_header;
    MB_mem_info* mem;

    if (mmap_tag == NULL) {
        printk("Multiboot memory info tag not found\n");
        return;
    }

    mem_header = (MB_tag_mmap*)mmap_tag;
    mem = mem_header->mem_info_entries;
    num_info = (mem_header->tag_size - MMAP_TAG_SIZE) / mem_header->mem_info_size;

    for (i = 0; i < num_info; i++) {
//        printk("Checking mem region between %p and %p\n", 
//            (void*)mem[i].region.base_addr, 
//            (void*)(mem[i].region.base_addr + mem[i].region.length - 1));
        
        if ((mem[i].type == 1)) {
//            printk("Type 1  mem region between %p and %p\n", 
//                (void*)mem[i].region.base_addr,
//                (void*)(mem[i].region.base_addr + mem[i].region.length - 1));
            find_avl_mem(mmap, mem[i].region);
        }
    }
}

void parse_tag_elf_symbols(mem_map* mmap, void* elf_tag) {
    uint32_t i, num_sects;
    MB_tag_elf_symbols* elf_header;
    MB_elf_sect_header* sects;
    mem_region region;
//    char *name, *str_table;

    if (elf_tag == NULL) {
        printk("Multiboot elf symbols tag not found\n");
        return;
    }

    elf_header = (MB_tag_elf_symbols*)elf_tag;
    sects = (MB_elf_sect_header*)(&(elf_header->sect_head_array));
    num_sects = elf_header->num_sect_head_entries;
//    str_table = (char*)elf_header->index_str_table_sect;

    for (i = 0; i < num_sects; i++) {
        region.base_addr = sects[i].seg_addr;
        region.length = sects[i].seg_size;

        if (region.length != 0) {
            printk("Unavailable mem region between %p and %p\n", 
                (void*)(sects[i].seg_addr), 
                (void*)(sects[i].seg_addr + sects[i].seg_size - 1));

            mmap->mem_unavl[mmap->num_unavl] = region;
            mmap->num_unavl++;
        }
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

void parse_mb_tags(mem_map* mmap, void* tag) {
    void* mem = NULL;
    void* bootl = NULL;
    void* elf = NULL;
    uint32_t type, size;

    tag += sizeof(MB_tag_header);
    
    while (!(((type = ((MB_tag*)tag)->tag_type) == TAG_TERM) 
            && (((MB_tag*)tag)->tag_size == 8))) {
//        printk("Tag #%d, Size = %d\n", type, ((MB_tag*)tag)->tag_size);

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
        tag += size;
    }

    parse_tag_bootloader(bootl);
    parse_tag_elf_symbols(mmap, elf);
    parse_tag_mmap(mmap, mem);
}

void init_mmap(mem_map* mmap) {
    mmap->num_avl = 0;
    mmap->num_unavl = 0;
    mmap->curr_region = 0;
    mmap->pg_last = 0;
    mmap->pgs_avl = 0;
    mmap->pgs_alloc = 0;
}

int add_pf() {
    mem_region r;
    uint64_t end;

    r = mmap.mem_avl[mmap.curr_region];
    end = r.base_addr + r.length - 1;

    mmap.pg_last += PG_SIZE;
BREAK;
    // check if room for a page in current region
    while (mmap.pg_last >= (void*)end) {
        mmap.curr_region++;

        if (mmap.curr_region == mmap.num_avl)
            return 0;

        r = mmap.mem_avl[mmap.curr_region];
        end = r.base_addr + r.length - 1;
        mmap.pg_last = (void*)r.base_addr;
    }

    ((pf_header*)mmap.pg_last)->next = mmap.pgs_free;
    mmap.pgs_free = mmap.pg_last;
    mmap.pgs_avl++;

    return 1;
}

void init_mem_pool() {
    mmap.pg_last = (void*)mmap.mem_avl[0].base_addr;

    while (mmap.pgs_avl != DFLT_NUM_PGS) {
        if(!add_pf())
            break;
    }
}

void MMU_setup(PML4_ref pml, void* tag) {
//    printk("Tag address: %p\n", tag);
//    printk("Header size: %d\n", ((MB_tag_header*)tag)->tag_size);
    init_mmap(&mmap);
    parse_mb_tags(&mmap, tag);
    init_mem_pool();
}

void* MMU_pf_alloc() {
    void* pf;

    // return NULL if no pages left to allocate
    if ((mmap.pgs_avl == 0) && (add_pf() == 0))
        return NULL;

    pf = mmap.pgs_free;
    mmap.pgs_free = ((pf_header*)mmap.pgs_free)->next;
    mmap.pgs_avl--;
    mmap.pgs_alloc++;

    return pf;
}

void MMU_pf_free(void* pf) {
    ((pf_header*)pf)->next = mmap.pgs_free;
    mmap.pgs_free = pf;
    mmap.pgs_alloc--;
    mmap.pgs_avl++;
}
