#include <stddef.h>
#include "multitask.h"
#include "kmalloc.h"
#include "mmu.h"
#include "strings.h"
#include "debug.h"

#define RFLAGS 0x2
#define CODE_SEGMENT 0x8

Process *head;
Process *curr_proc, *next_proc;

void* dead_kstack;

int pid_g;

void PROC_run() {
//    BREAK; 
    if (curr_proc != NULL)
        yield();
}

void PROC_reschedule() {
    next_proc = (curr_proc->next == NULL) ? head : curr_proc->next;
}

Process* PROC_create_kthread(kproc_t entry, void* arg) {
    Process* p = (Process*)kmalloc(sizeof(Process));
    memset((void*)p, 0, sizeof(Process));

    p->regs.rip = (uint64_t)(void*)entry;
    p->regs.rdi = (uint64_t)arg;
    p->pid = pid_g++;

    if (curr_proc == NULL) {
        curr_proc = p;
        head = p;

        head->next = NULL;
        head->prev = NULL;
    }
    else {
        p->kstack = MMU_alloc_kstack();
        p->regs.rsp = (uint64_t)p->kstack;
        p->regs.rbp = p->regs.rsp;
        p->regs.rflags = RFLAGS;
        p->regs.cs = CODE_SEGMENT;

        p->next = head;
        head->prev = p;
        head = p;
    }

    return p;
}

void PROC_init() {
    head = NULL;
    curr_proc = NULL;
    next_proc = NULL;
    dead_kstack = NULL;
    pid_g = 0;

    PROC_create_kthread(NULL, NULL);
}

void PROC_yield(uint64_t rsp) {
    PROC_reschedule();

    memcpy((void*)curr_proc, (void*)rsp, sizeof(Proc_regs));
    curr_proc = next_proc;
    memcpy((void*)rsp, (void*)curr_proc, sizeof(Proc_regs));
}

void PROC_exit(uint64_t rsp) {
    Process* dead_proc = curr_proc;

    // Frees old stack
    if (dead_kstack != NULL)
        MMU_free_kstack(dead_kstack);

    if (curr_proc->prev != NULL)
        curr_proc->prev->next = curr_proc->next;
    else
        head = curr_proc->next;

    if (curr_proc->next != NULL)
        curr_proc->next->prev = curr_proc->prev;

    PROC_reschedule();

    curr_proc = next_proc;
    memcpy((void*)rsp, (void*)curr_proc, sizeof(Proc_regs)); 

    dead_kstack = dead_proc->kstack;
    kfree(dead_proc);
}
