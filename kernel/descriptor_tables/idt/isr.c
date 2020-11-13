//
// isr.c -- High level interrupt service routines and interrupt request handlers.
//

#include "isr.h"
#include "../../../drivers/VGA/VGA_text.h"
#include "../../../utils/int_utils.h"
#include "../../../utils/serial_ports.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
   vga_write_string("recieved interrupt: ");

   char* interruptNumber = "   ";
   itoa(regs.interrupt_number, interruptNumber, 10);
   vga_write_string(interruptNumber);

   vga_write_string("\n");
}

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (regs.interrupt_number >= 40)
   {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.interrupt_number] != 0)
   {
       isr_t handler = interrupt_handlers[regs.interrupt_number];
       handler(regs);
   }
}