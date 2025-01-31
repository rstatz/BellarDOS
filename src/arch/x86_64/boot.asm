global start

extern long_mode_start

global stack_ist1_top
global stack_ist2_top
global stack_ist3_top
global stack_ist4_top
global stack_ist5_top
global stack_ist6_top
global stack_ist7_top

global stack_rsp0_top
global stack_rsp1_top
global stack_rsp2_top

section .rodata
gdt64:
    dq 0
.code: equ $ - gdt64
    ; sets code label to table offset
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53)
.pointer:
    dw $ - gdt64
    dq gdt64

section .text
bits 32

start:
    ; set stack address
    mov esp, stack_top

    push ebx

    call check_multiboot
    call check_cpuid
    call check_long_mode

    call set_up_page_tables
    call enable_paging

    lgdt [gdt64.pointer]

    jmp gdt64.code:long_mode_start

    ; print 'OK'
    mov dword [0xb8000], 0x2f4b2f4f
    hlt

check_multiboot:
    ; checks if booting in multiboot
    cmp eax, 0x36d76289
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "0"
    jmp error


check_cpuid:
    ; copy flags to EAX via stack
    pushfd
    pop eax

    ; copy to ECX as well for comparing later 
    mov ecx, eax

    ; Flip the ID bit
    xor eax, 1 << 21

    ; Copy EAX to FLAGS via stack
    push eax
    popfd

    ; Copy FLAGS back to the EAX
    pushfd
    pop eax

    ; Restore FLAGS from ECX
    push ecx
    popfd
    
    ; Compare EAX and ECX (if equal, CPUID not supported)
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "1"
    jmp error

check_long_mode:
    ; test if extended processor info is available
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode

    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_long_mode
    ret
.no_long_mode:
    mov al, "2"
    jmp error

set_up_page_tables:
    ; map first P4 entry to P3 table
    mov eax, p3_table
    or eax, 0b11
    mov [p4_table], eax

    ; map first p3 entry to p2 table
    mov eax, p2_table
    or eax, 0b11
    mov [p3_table], eax

    ; create counter
    mov ecx, 0

.map_p2_table:
    ; map ecx-th P2 entry to page at 2MiB*ecx
    mov eax, 0x200000
    mul ecx
    or eax, 0b10000011
    mov [p2_table + ecx * 8], eax

    inc ecx
    cmp ecx, 512
    jne .map_p2_table

    ret

enable_paging:
    ; load P4 to cr3 register
    mov eax, p4_table
    mov cr3, eax

    ; enable PAE-flag in cr4
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; set long mode bit in EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr
    
    ; enable paging in cr0
    mov eax, cr0
    or eax, 1 <<31
    mov cr0, eax

    ret

; prints error
error:
    ; prints error
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov byte  [0xb800a], al
    hlt

section .bss
align 4096
p4_table:
    resb 4096
p3_table:
    resb 4096
p2_table:
    resb 4096

stack_ist1:
    resb 4096
stack_ist1_top:

stack_ist2:
    resb 4096
stack_ist2_top:

stack_ist3:
    resb 4096
stack_ist3_top:

stack_ist4:
    resb 4096
stack_ist4_top:

stack_ist5:
    resb 4096
stack_ist5_top:

stack_ist6:
    resb 4096
stack_ist6_top:

stack_ist7:
    resb 4096
stack_ist7_top:

stack_rsp0:
    resb 4096
stack_rsp0_top:

stack_rsp1:
    resb 4096
stack_rsp1_top:

stack_rsp2:
    resb 4096
stack_rsp2_top:

stack_bottom:
    resb 4096
stack_top:

