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
        f.write(s)

    s =  ("\n"
         "isr_normal:\n"
#         "      call bpoint\n"
#         "      hlt\n"
         "      call interrupt_handler\n"
         "      pop rdi\n"
#         "      sti\n"
         "      iretq\n"
         "\n"
         "isr_err:\n"
#         "      call bpoint\n"
#         "      hlt\n"
         "      call interrupt_handler_err\n"
         "      pop rsi\n"
         "      pop rdi\n"
         "      add rbp, 8\n"
#         "      mov rsp, rbp\n"
#         "      sti\n"
         "      iretq\n\n")
    f.write(s)

    for i in range(0, 256) :
        if (isr_err[i] == 1) :
            s = ("isr%d:\n" %i +
                 "      call bpoint\n"
                 "      push rdi\n"
                 "      push rsi\n"
                 "      mov rdi, 0x%x\n" % i +
                 "      mov rsi, [rbp]\n"
                 "      jmp isr_err\n\n")
        else :
            s = ("isr%d:\n" % i +
                 "      call bpoint\n"
                 "      push rdi\n"
                 "      mov rdi, 0x%x\n" % i +
                 "      jmp isr_normal\n\n")
        f.write(s)

    s = ("isr_unsupported:\n"
         "      call bpoint\n"
         "      push rdi\n"
         "      mov rdi, 255\n"
         "      jmp isr_normal\n\n")
    f.write(s)

if __name__ == "__main__":
    gen_isr() 
