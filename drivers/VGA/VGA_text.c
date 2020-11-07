#include "VGA_text.h"
#include "../../utils/string_utils.h"
#include "../../utils/serial_ports.h"

#include <stdbool.h>
#include <stdint.h>

size_t vga_row;
size_t vga_column;
uint8_t vga_default_color;
uint16_t* vga_buffer;

static inline uint8_t vga_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 

static inline uint16_t vga_entry(unsigned char c) 
{
	return (uint16_t) c | (uint16_t) vga_default_color << 8;
}

void vga_initialize(void) 
{
	vga_row = 0;
	vga_column = 0;
	vga_default_color = vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	vga_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) { // Empty the screen
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			vga_buffer[index] = vga_entry(' ');
		}
	}
}
 
void vga_set_color(uint8_t color) 
{
	vga_default_color = color;
}

void vga_put_char_at(char c, size_t row, size_t col) 
{
	const size_t index = row * VGA_WIDTH + col;
	vga_buffer[index] = vga_entry(c);
}

void log(char c)
{
	outb(0x3F8, c);
}

void log_string(char* str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++) log(str[i]);
}

char get_char(size_t row, size_t col)
{
	size_t index = row * VGA_WIDTH + col;
	uint16_t entry = vga_buffer[index];
	char c = ((char)(((uint32_t)(entry)) & 0xFF)); // Get the lower bytes

	return c;
}

void scroll()
{
	for (size_t row = 1; row < VGA_HEIGHT; row++) // Start from the second row
	{
		for (size_t col = 0; col < VGA_WIDTH; col++) // Itereate every char
		{
			char c = get_char(row, col);
			vga_put_char_at(c, row-1, col);

			if (row == LAST_ROW)
			{
				// Fill last row with spaces
				vga_put_char_at(' ', row, col);
			}
		}
	}
}

/**
 * Scrolls the screen if needed
 */
void handle_scroll()
{
	if (vga_row == VGA_HEIGHT) // If we are going to write in the 26 row, which is not exists..
	{
		scroll();
		vga_row = LAST_ROW; // Go to the last row
	}
}

void new_line()
{
	vga_row++;
	vga_column = 0;
}

/**
 * Treats a case that a new line is needed
 */
void handle_line_end()
{
	if (vga_column == VGA_WIDTH-1) new_line();
}

/**
 * Checks if this char is special. If yes, it treats it's case, and returns true.
 * Else, returns false.
 * // TODO: Handle other chars
 */
bool handle_special_chars(char c)
{
	switch (c)
	{
		case '\n': // New line:
			new_line();
			
			return true;
		default:
			return false;
	}
}

void vga_put_char(char c)
{	
	handle_line_end();
	handle_scroll();
	if (handle_special_chars(c)) return; // We have already treated the special char

    vga_put_char_at(c, vga_row, vga_column);
	vga_column++; // Go to next char
}
 
void vga_write_string(const char* str) 
{
	for (size_t i = 0; i < strlen(str); i++)
		vga_put_char(str[i]);
}