#include "vga.h"

unsigned char inb(unsigned short port) {
    unsigned char result;

    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

const char scancode_to_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 0x00 - 0x09 */
  '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',  /* 0x0A - 0x13 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',    0,  /* 0x14 - 0x1D */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',    /* 0x1E - 0x27 */
 '\'', '`',   0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',   /* 0x28 - 0x31 */
  'm', ',', '.', '/',   0, '*',   0, ' ',   0          /* 0x32 - 0x3B */
};

char get_chr(void) {
    unsigned char scancode;

    while (1) {
        if (inb(0x64) & 1){

            scancode = inb(0x60);

            if (scancode & 0x80) {
                continue;
            }

            if (scancode < sizeof(scancode_to_ascii)) {
                char ascii = scancode_to_ascii[scancode];
                if (ascii != 0) {
                    return ascii;
                }
            }
        }
    }
}

void printchar(char c, uint8_t color) {
    if (c == '\n') {
        col = 0;
        row++;
        return;
    }

    vga_buffer[row * VGA_WIDTH + col] = ((uint16_t)color << 8) |(uint8_t)c;
    col++;

    if (col >= VGA_WIDTH) {
        col = 0;
        row++;
    }
}

void backspace(uint8_t color, uint8_t min_col) {
    if (col > min_col) {
        col--;

        vga_buffer[row * VGA_WIDTH + col] = ((uint16_t)color << 8) | ' ';
    }
}

void print(const char* str, uint8_t color) {

    while (*str != '\0') {
        printchar(*str++, color);
    }

}

void print_int(int number, uint8_t color) {
    char buffer[12];
    int i = 0;

    while (number > 0) {
        buffer[i++] = '0' + (number % 10);
        number /= 10;
    }

    while (i > 0) {
        printchar(buffer[--i], color);
    }
}

void clear_screen(uint8_t color) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = ((uint16_t)color << 8) | ' ';
    }

    row = 0;
    col = 0;
}

void delay(unsigned long count) {
    volatile unsigned long i;
    for (i = 0; i < count; i++) {
        __asm__ volatile("nop");
    }
}

void lock_screen(void) {
    clear_screen(0x0B);

    print("================KOY_OS==================\n\n", 0xF1);
    
    uint8_t input_start_col = col;
        while (1) {
            print("Password: ", 0x0F);

            char pass_input[PASS_INPUT_SIZE];
            int i = 0;

            while (1) {
                char c = get_chr();

                if (c == '\n' || c == '\r') {
                    pass_input[i] = '\0';

                    print('\n', 0x0F);

                    break;
                }

                if (c == '\b') {
                if (i > 0) {
                    i--;
                    backspace(0x0F, input_start_col);
                }
                continue;
            }

                if ( i < PASS_INPUT_SIZE - 1) {
                    pass_input[i] = c;
                    i++;

                    printchar('*', 0x04);
                }
            }

            if (koy_strcmp(pass_input, PASSWORD) == 0) {
                print("\nPassword Correct!!\n", 0x0B);
                clear_screen(0x0F);
                print("KOY_OS\n\n", 0x0B);
                print("Made By Dhawal Ryl.\n\n\n", 0x0E);
                break;
            } else {
                print("\nIncorrect Password!!\n", 0x04);
            }
        }
    }

static inline void outw(uint16_t port, uint16_t value) {
    __asm__ volatile (
        "outw %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

void shutdown(void) {
    outw(0x604, 0x2000);

    for (;;) {
        __asm__ volatile ("cli");
        __asm__ volatile ("hlt");
    }
}

void scroll_screen(uint8_t color) {

    // Move every row one row upward
    for (int row_index = 1; row_index < VGA_HEIGHT; row_index++) {

        for (int col_index = 0; col_index < VGA_WIDTH; col_index++) {

            vga_buffer[
                (row_index - 1) * VGA_WIDTH + col_index
            ] =
            vga_buffer[
                row_index * VGA_WIDTH + col_index
            ];
        }
    }

    // Clear the last row
    for (int col_index = 0; col_index < VGA_WIDTH; col_index++) {

        vga_buffer[
            (VGA_HEIGHT - 1) * VGA_WIDTH + col_index
        ] =
        ((uint16_t)color << 8) | ' ';
    }

    row = VGA_HEIGHT - 1;
    col = 0;
}