#include "../drivers/VGA/VGA_text.h"
#include "../utils/int_utils.h"
#include "descriptor_tables/descriptor_tables.h"

void kernel_main(void) 
{	
	vga_initialize();
	vga_clean_screen();

	init_descriptor_tables();

	for (int i = 0; i < 501; i++)
	{
		char* str = "   ";
		itoa(i, str, 10);
		vga_write_string(str);
		vga_write_string("\n");
	}
}