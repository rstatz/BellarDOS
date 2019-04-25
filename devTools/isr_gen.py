def gen_isr() :
    i = 0;

    f = open("isr.asm", "w+")

    s = ("extern interrupt_handler\n"
         "extern interrupt_handler_err\n"
         "\n"
         "isr_normal:\n"
         "      call interrupt_handler\n"
         # Restore regs
         "      pop rdi\n"
         "      iretq\n")
    f.write(s);

    for i in range(0, 256) :
        s = ("isr%d:\n" % i +
             "    push rdi\n" +
             "    mov 0x%x, rdi\n" % i +
             "    jmp isr_normal\n\n")
        f.write(s);

if __name__ == "__main__":
    gen_isr() 
