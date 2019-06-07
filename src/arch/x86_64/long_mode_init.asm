global long_mode_start
section .text
bits 64

extern kmain

long_mode_start:
    pop rdi

    ; load 0 to all data segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; print okay
    mov rax, 0x2f592f412f4b2f4f
    mov qword [0xb8000], rax

    call kmain

    hlt

