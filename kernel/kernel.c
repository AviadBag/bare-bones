#include "../drivers/VGA/VGA.h"

void kernel_main(void) 
{
	/* Initialize vga interface */
	vga_initialize();

	for (int i = 0; i < 501; i++)
		vga_write_string("Aviad");
}