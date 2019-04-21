def gen_isr() :
    i = 0;

    f = open("isr.asm", "w+")

    s = ("isr_normal:\n"
         "      \n")

    for i in range(0, 256) :
        s = ("isr%d:\n" % i +
             "    push rdi\n" +
             "    mov 0x%x, rdi\n" % i +
             "    jmp isr_normal\n\n")
        f.write(s);

if __name__ == "__main__":
    gen_isr() 
