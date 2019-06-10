#include <stdint.h>

#ifndef MULTITASK_H
#define MULTITASK_H

typedef struct Proc {
    uint32_t pid;
    uint64_t rax, rbx, rcx, rdx, rdi, rsi;
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t cs, ss, ds, es, fs, gs;
    uint64_t rbp, rsp, rip, rflags;
} Proc;

typedef void (*kproc_t)(void*);

void PROC_run();
void PROC_create_kthread(kproc_t entry_point, void* arg);
void PROC_reschedule();
void PROC_yield();
void kexit();

static inline void yield() {
    asm volatile("INT $116");
}

#endif
