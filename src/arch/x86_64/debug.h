#ifndef DEBUG_H
#define DEBUG_H

#define BREAK bpoint()
#define HLT halt_cpu()

void bpoint();

void halt_cpu();

#endif
