#include "../drivers/VGA/VGA_text.h"
#include "../utils/int_utils.h"

void kernel_main(void) 
{
	/* Initialize vga interface */
	vga_initialize();

	vga_clean_screen();

	for (int i = 0; i < 501; i++)
	{
		char* str;
		itoa(i, str, 10);
		vga_write_string(str);
		vga_write_string("\n");
	}
}