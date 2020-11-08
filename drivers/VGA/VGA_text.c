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
}

/**
 * Moves the cursor to the place indicated by vga_row and vga_column
 */
static void move_cursor()
{
   // The screen is 80 characters wide...
   uint16_t cursorOffset = vga_row * 80 + vga_column;
   outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
   outb(0x3D5, cursorOffset >> 8); // Send the high cursor byte.
   outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
   outb(0x3D5, cursorOffset);      // Send the low cursor byte.
} 

/**
 * Cleanse up the screen
 */
void vga_clean_screen()
{
	for (size_t row = 0; row < VGA_HEIGHT; row++) { // Empty the screen
		for (size_t column = 0; column < VGA_WIDTH; column++) {
			vga_put_char_at(' ', row, column);
		}
	}
}

/**
 * Returns the char in the given position
 */
static char get_char(size_t row, size_t col)
{
	size_t index = row * VGA_WIDTH + col;
	uint16_t entry = vga_buffer[index];
	char c = ((char)(((uint32_t)(entry)) & 0xFF)); // Get the lower bytes

	return c;
}

/**
 * Scrolls the screen
 */
static void scroll()
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
static void handle_scroll()
{
	if (vga_row == VGA_HEIGHT) // If we are going to write in the 26 row, which is not exists..
	{
		scroll();
		vga_row = LAST_ROW; // Go to the last row
	}
}

/**
 * Enters to a new line
 */
static void new_line()
{
	vga_row++;
	vga_column = 0;
}

/**
 * Treats a case that a new line is needed
 */
static void handle_line_end()
{
	if (vga_column == VGA_WIDTH-1) new_line();
}

/**
 * Checks if this char is special. If yes, it treats it's case, and returns true.
 * Else, returns false.
 * // TODO: Handle other chars
 */
static bool handle_special_chars(char c)
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

/**
 * Puts a char in the next place, and increments the column to the next place.
 */
static void vga_put_char(char c)
{	
	handle_line_end();
	handle_scroll();
	if (handle_special_chars(c)) return; // We have already treated the special char

    const size_t index = vga_row * VGA_WIDTH + vga_col;
	vga_buffer[index] = vga_entry(c);
	
	vga_column++; // Go to next char
	move_cursor(); // Move cursor to next column
}
 
/**
 * Writes a screen to the screen
 */
void vga_write_string(const char* str) 
{
	for (size_t i = 0; i < strlen(str); i++)
	{
		vga_put_char(str[i]);
	}
}