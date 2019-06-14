#include <stddef.h>
#include "multitask.h"
#include "kmalloc.h"
#include "mmu.h"
#include "irq.h"

#include "strings.h"
#include "debug.h"

#define RFLAGS 0x2
#define CODE_SEGMENT 0x8

Proc_q rdy_q;

Process* next_proc;

void* dead_kstack;

int pid_g;

void unlink_curr() {
    if (curr_proc->prev != NULL)
        curr_proc->prev->next = curr_proc->next;
    else
        rdy_q.head = curr_proc->next;

    if (curr_proc->next != NULL)
        curr_proc->next->prev = curr_proc->prev;
}

Process* unlink_head(Proc_q* q) {
    Process* p;

    if ((q == NULL) || (q->head == NULL))
        return NULL;

    p = q->head;
    
    if ((q->head = p->next) != NULL)
        p->next->prev = NULL;

    p->next = NULL;

    return p;
}

void append_proc(Process* p, Proc_q* q) {
    p->next = q->head;
    p->prev = NULL;

    if (q->head != NULL)
        q->head->prev = p;

    q->head = p;
}

void PROC_run() {
    if (curr_proc != NULL)
        yield();
}

void PROC_reschedule() {
    next_proc = (curr_proc->next == NULL) ? rdy_q.head : curr_proc->next;
}

Process* PROC_create_kthread(kproc_t entry, void* arg) {
    Process* p = (Process*)kmalloc(sizeof(Process));
    memset((void*)p, 0, sizeof(Process));

    p->regs.rip = (uint64_t)(void*)entry;
    p->regs.rdi = (uint64_t)arg;
    p->pid = pid_g++;

    if (curr_proc == NULL) {
        curr_proc = p;
        rdy_q.head = p;

        rdy_q.head->next = NULL;
        rdy_q.head->prev = NULL;
    }
    else {
        p->kstack = MMU_alloc_kstack();
        p->regs.rsp = (uint64_t)p->kstack;
        p->regs.rbp = p->regs.rsp;
        p->regs.rflags = RFLAGS;
        p->regs.cs = CODE_SEGMENT;

        p->next = rdy_q.head;
        rdy_q.head->prev = p;
        rdy_q.head = p;
    }

    return p;
}

void PROC_init_q(Proc_q* q) {
    q->head = NULL;
}

void PROC_init() {
    PROC_init_q(&rdy_q);

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

    unlink_curr();

    PROC_reschedule();

    curr_proc = next_proc;
    memcpy((void*)rsp, (void*)curr_proc, sizeof(Proc_regs)); 

    dead_kstack = dead_proc->kstack;
    kfree(dead_proc);
}

void PROC_block_on(Proc_q* q, int enable_ints) {
    if (q == NULL)
        return;

    unlink_curr();
    append_proc(curr_proc, q);

    if (enable_ints)
        STI;

    yield();
}

void PROC_unblock_head(Proc_q* q) {
    Process* p;
    
    if ((q == NULL) || (q->head == NULL))
        return;

    p = unlink_head(q);
    append_proc(p, &rdy_q);
}

void PROC_unblock_all(Proc_q* q) {
    if (q == NULL)
        return;

    while(q->head != NULL) PROC_unblock_head(q);
}
