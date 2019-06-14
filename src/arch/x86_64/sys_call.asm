extern bpoint
extern interrupt_handler

global isr115
global isr116

sys_call_ret:
    pop gs
    pop fs
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    iretq

; Exit
isr115:
    push rax
    push rax
    push rax
    push rax
    push rax

    push rax
    push rbx
    push rcx
    push rdx
    push rdi
    push rsi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push fs
    push gs  

    mov rdi, 115
    mov rsi, rsp
    
    call interrupt_handler
    
    jmp sys_call_ret

; Yield
isr116:   
    push rax
    push rbx
    push rcx
    push rdx
    push rdi
    push rsi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push fs
    push gs

    mov rdi, 116
    mov rsi, rsp 

    call interrupt_handler

    jmp sys_call_ret
