#include "../drivers/VGA/VGA_text.h"
#include "../utils/int_utils.h"
#include "descriptor_tables/gdt/gdt.h"
#include "descriptor_tables/idt/idt.h"

void kernel_main(void) 
{	
	initialize_vga();
	vga_clean_screen();

	init_gdt();
	init_idt();
}