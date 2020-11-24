// timer.c -- Initialises the PIT, and handles clock updates.
// Written for JamesM's kernel development tutorials.

#include "timer.h"
#include "../descriptor_tables/idt/isr.h"
#include "../../drivers/VGA/VGA_text.h"
#include "../../include/registers.h"
#include "../../utils/serial_ports.h"
#include "../../utils/int_utils.h"

uint32_t ticks_counter = 0;

static void timer_callback(registers_t regs)
{
    ticks_counter++;
    
    vga_write_string("Tick: ");
    char *ticks_counter_str = "     ";
    itoa(ticks_counter, ticks_counter_str, 10);
    vga_write_string(ticks_counter_str);
    vga_write_string("\n");
}

void init_timer(uint32_t frequency)
{
    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ0, &timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(PIT_COMMAND_PORT, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t divisor_lower_byte = (uint8_t)(divisor & 0xFF);
    uint8_t divisor_higher_byte = (uint8_t)((divisor >> 8) & 0xFF);

    // Send the frequency divisor.
    outb(PIT_DATA_PORT, divisor_lower_byte);
    outb(PIT_DATA_PORT, divisor_higher_byte);
}