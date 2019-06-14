#include <stdint.h>

#ifndef MULTITASK_H
#define MULTITASK_H

//typedef struct Proc_regs {
//    uint64_t ss, rsp, rflags, cs, rip;
//    uint64_t rax, rbx, rcx, rdx, rdi, rsi, rbp;
//    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
//    uint64_t fs, gs;
//} __attribute__((packed)) Proc_regs;

typedef struct Proc_regs {
    uint64_t gs, fs;
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rsi, rdi, rdx, rcx, rbx, rax;
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) Proc_regs;

typedef struct Proc {
    Proc_regs regs;

    void* kstack;
    uint32_t pid;
    struct Proc *next, *prev;
} Proc;

void PROC_init();

void PROC_run();

typedef void (*kproc_t)(void*);
void PROC_create_kthread(kproc_t entry_point, void* arg);

void PROC_reschedule();

void PROC_yield(uint64_t);

void PROC_exit(uint64_t);

static inline void yield() {
    asm volatile("INT $116");
}

static inline void exit() {
    asm volatile("INT $115");
}

#endif
