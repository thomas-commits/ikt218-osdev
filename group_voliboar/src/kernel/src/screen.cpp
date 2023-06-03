#include "screen.h"
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

// Will set foreground and background color of the terminal
void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

// Will put a character at given position
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	// Custom code. Will move "cursor" to next line if '\n'
	// Does not continue printing after newline
	if(c == '\n' && terminal_row < VGA_HEIGHT)
	{
		terminal_row++;
		terminal_column = -1;	// Will be incremented by 1, so set to -1 instead of 0
		return;
	}

	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

// Abstracted version of "putentryat"
void terminal_putchar(char c) 
{
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

// Will calculate the string size according to string size
void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

// Will print the given string
void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}
 
 // SOURCE:
 // https://wiki.osdev.org/Bare_Bones#Implementing_the_Kernel