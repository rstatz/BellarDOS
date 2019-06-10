#include "multitask.h"

Proc curr_proc, next_proc;

void PROC_run();

void PROC_reschedule() {

}

void PROC_create_kthread(kproc_t entry, void* arg);

void PROC_yield() {
    PROC_reschedule();

    
}
