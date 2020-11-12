//
// isr.c -- High level interrupt service routines and interrupt request handlers.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include "isr.h"
#include "../../../drivers/VGA/VGA_text.h"
#include "../../../utils/int_utils.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
   vga_write_string("recieved interrupt: ");

   char* interruptNumber = "   ";
   itoa(regs.int_no, interruptNumber, 10);
   vga_write_string(interruptNumber);

   vga_write_string("\n");
}