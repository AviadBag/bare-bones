#include "../drivers/VGA/VGA_text.h"
#include "../utils/int_utils.h"
#include "descriptor_tables/gdt/gdt.h"
#include "descriptor_tables/idt/idt.h"
#include "../drivers/timer/timer.h"

void tick(int counter)
{
    vga_write_string("Tick: ");
    char *ticks_counter_str = "     ";
    itoa(counter, ticks_counter_str, 10);
    vga_write_string(ticks_counter_str);
    vga_write_string("\n");
}

void kernel_main(void) 
{	
	vga_initialize();
	vga_clean_screen();

	init_gdt();
	init_idt();
	init_timer(100);
	register_timer_callback(tick);

	while (1) {}
}
