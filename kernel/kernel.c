#include "../drivers/VGA/VGA_text.h"
#include "../utils/int_utils.h"
#include "descriptor_tables/gdt.h"
#include "descriptor_tables/idt.h"

void kernel_main(void) 
{	
	vga_initialize();
	vga_clean_screen();

	init_gdt();
	init_idt();

	asm volatile ("int $0x3");
	asm volatile ("int $0x4");
}