/* Based on https://wiki.osdev.org/Inline_Assembly/Examples */

#include <stdint.h>
#include <stdbool.h>
#include <cpuid.h>

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

static inline bool are_interrupts_enabled() {
    unsigned long flags;
    asm volatile("pushf\n\t"
                 "pop %0"
                 : "=g"(flags));
    return flags & (1 << 9);
}

static inline unsigned long save_irqdisable(void) {
    unsigned long flags;
    asm volatile("pushf\n\tcli\n\tpop %0" : "=r"(flags) : : "memory");
    return flags;
}

static inline void restore_irqenable(unsigned long flags) {
    asm ("push %0\n\tpopf" : : "rm"(flags) : "memory","cc");
}

static inline void lidt(void *base, uint16_t size) {
    struct {
        uint16_t length;
        void *base;
    } __attribute__((packed)) IDTR = {size, base};
    asm("lidt %0" : : "m"(IDTR));
}

int poll_keyboard(void) {
    while ((inb(0x64) & 1) == 0) {
        ;
    }
    return inb(0x60);
}

int get_cpu_vendor_string(char vendor_string[13]) {
    unsigned int eax, ebx, ecx, edx;
    eax = 0;

    int status = __get_cpuid(0, &eax, &ebx, &ecx, &edx);
    if (!status) {
        return 0;
    }

    int i = 0;
    vendor_string[i++] = *(((char *)&ebx) + 0);
    vendor_string[i++] = *(((char *)&ebx) + 1);
    vendor_string[i++] = *(((char *)&ebx) + 2);
    vendor_string[i++] = *(((char *)&ebx) + 3);
    vendor_string[i++] = *(((char *)&edx) + 0);
    vendor_string[i++] = *(((char *)&edx) + 1);
    vendor_string[i++] = *(((char *)&edx) + 2);
    vendor_string[i++] = *(((char *)&edx) + 3);
    vendor_string[i++] = *(((char *)&ecx) + 0);
    vendor_string[i++] = *(((char *)&ecx) + 1);
    vendor_string[i++] = *(((char *)&ecx) + 2);
    vendor_string[i++] = *(((char *)&ecx) + 3);
    vendor_string[i] = '\0';

    return 1;
}
