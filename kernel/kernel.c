#include "../drivers/VGA/VGA_text.h"

void kernel_main(void) 
{
	/* Initialize vga interface */
	vga_initialize();

	vga_clean_screen();

	// for (int i = 0; i < 501; i++)
	// 	vga_write_string("Aviad");
}