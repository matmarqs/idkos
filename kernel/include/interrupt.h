#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <stdint.h>

struct cpu_state {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t esp;
} __attribute__((packed));

typedef struct cpu_state cpu_state;

struct intr_state {
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} __attribute__((packed));

typedef struct intr_state intr_state;

void interrupt_handler(cpu_state cpu, uint32_t intr_num, intr_state intr);

#endif /* _INTERRUPT_H */
