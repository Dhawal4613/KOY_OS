#ifndef VGA_H
#define VGA_H

#define VGA_BUFFER 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define PASS_INPUT_SIZE 5
#define PASSWORD "2209"


#include <stdint.h>

static volatile uint16_t *vga_buffer = (volatile uint16_t*)VGA_BUFFER;
static uint16_t cursor = 0;
static uint8_t row = 0;
static uint8_t col = 0;

unsigned char inb(unsigned short port);
char get_char(void);
void printchar(char c, uint8_t color);
void backspace(uint8_t color, uint8_t min_col);
void print(const char* str, uint8_t color);
void print_int(int number, uint8_t color);
void clear_screen(uint8_t color);
void delay(unsigned long count);
void lock_screen(void);
static inline void outw(uint16_t port, uint16_t value);
void shutdown(void);
#endif