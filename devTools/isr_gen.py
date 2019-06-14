def gen_isr() :
    isr_err = []

    for i in range (0, 256):
        isr_err.append(0)

    isr_err[8] = 1
    isr_err[10] = 1
    isr_err[11] = 1
    isr_err[12] = 1
    isr_err[13] = 1
    isr_err[14] = 1
    isr_err[17] = 1
    isr_err[30] = 1
    
    i = 0

    f = open("src/arch/x86_64/isr.asm", "w+")

    s = ("extern bpoint\n"
         "extern interrupt_handler\n"
         "extern interrupt_handler_err\n\n"
         "global isr_unsupported\n\n")
    
    f.write(s)

    for i in range(0, 256) :
        s = ("global isr%d\n" % i)
        if (i is not 116 and i is not 115) :
            f.write(s)

    s =  ("\n"
         "isr_normal:\n"
#         "      call bpoint\n"
         "      call interrupt_handler\n"
         "      pop rax\n"
         "      pop r9\n"
         "      pop r8\n"
         "      pop rcx\n"
         "      pop rdx\n"
         "      pop rsi\n"
         "      pop rdi\n"
         "      pop r11\n"
         "      pop r10\n"
         "      iretq\n"
         "\n"
         "isr_err:\n"
#         "      call bpoint\n"
         "      call interrupt_handler_err\n"
         "      pop rax\n"
         "      pop r9\n"
         "      pop r8\n"
         "      pop rcx\n"
         "      pop rdx\n"
         "      pop rsi\n"
         "      pop rdi\n"
         "      pop r11\n"
         "      pop r10\n"
         "      pop r15\n"
         "      iretq\n\n")
    f.write(s)

    for i in range(0, 256) :
        if (isr_err[i] == 1) :
            s = ("isr%d:\n" %i +
#                 "      call bpoint\n"
                 "      push r10\n"
                 "      push r11\n"
                 "      push rdi\n"
                 "      push rsi\n"
                 "      mov rsi, [rsp + 32]\n"
                 "      push rdx\n"
                 "      push rcx\n"
                 "      push r8\n"
                 "      push r9\n"
                 "      push rax\n"
                 "      mov rdi, 0x%x\n" % i +
                 "      jmp isr_err\n\n")
        else :
            s = ("isr%d:\n" % i +
#                 "      call bpoint\n"
                 "      push r10\n"
                 "      push r11\n"
                 "      push rdi\n"
                 "      push rsi\n"
                 "      push rdx\n"
                 "      push rcx\n"
                 "      push r8\n"
                 "      push r9\n"
                 "      push rax\n"
                 "      mov rdi, 0x%x\n" % i +
                 "      jmp isr_normal\n\n")
        f.write(s)

    s = ("isr_unsupported:\n"
#         "      call bpoint\n"
         "      push rdi\n"
         "      mov rdi, 255\n"
         "      jmp isr_normal\n\n")
    f.write(s)

if __name__ == "__main__":
    gen_isr() 

