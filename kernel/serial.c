#include "io.h"
#include "serial.h"

static unsigned short global_com;

int serial_init(unsigned short com) {
   global_com = com;
   outb(global_com + 1, 0x00);    // Disable all interrupts
   outb(global_com + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(global_com + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(global_com + 1, 0x00);    //                  (hi byte)
   outb(global_com + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(global_com + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(global_com + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(global_com + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

   // Check if serial is faulty (i.e: not same byte as sent)
   if (inb(global_com + 0) != 0xAE) {
      return 1;
   }

   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs DISABLED and OUT#1 and OUT#2 bits DISABLED. RTS/DTR set)
   outb(global_com + 4, 0x03);
   return 0;
}

static int serial_is_transmit_fifo_empty() {
    return inb(global_com + 5) & 0x20;
}

void serial_putchar(char c) {
    while (serial_is_transmit_fifo_empty() == 0); /* spins until fifo is ready */
    outb(global_com, c);
}
