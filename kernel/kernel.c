#include "../drivers/VGA/VGA_text.h"
#include "../utils/int_utils.h"
#include "descriptor_tables/gdt/gdt.h"
#include "descriptor_tables/idt/idt.h"
#include "timer/timer.h"

void kernel_main(void) 
{	
	asm("sti");

	vga_initialize();
	vga_clean_screen();

	init_gdt();
	init_idt();
	init_timer(100);

	while (1) {}
}