#include "include/vga.h"
#include "include/koy_string.h"

#define INPUT_SIZE 1025
#define TEXT_INPUT_SIZE 1024 * 1024 * 1024

uint8_t input_start_col;
// int input_start_row;
void kernel_main(void) {

    lock_screen();

    clear_screen(0x0B);

    print("KOY_OS\n\n", 0x0B);
    print("Made By Dhawal Ryl.\n\n\n", 0x0E);

    while (1) {

        print("DHAWAL", 0x0B);
        print("]", 0x0C);
        print("koy>", 0x02);

        input_start_col = col;
        // input_start_row = row;

        char input[INPUT_SIZE];
        int i = 0;
        while (1) {

            char c = get_chr();

            if (c == '\n' || c == '\r') {

                input[i] = '\0';

                print("\n", 0x0F);

                break;
            }

            if (c == '\b') {
                if (i > 0) {
                    i--;
                    backspace(0x0F, input_start_col);
                }
                continue;
            }

            if (i < INPUT_SIZE - 1) {

                input[i] = c;
                i++;

                printchar(c, 0x0F);
            }
        }

        if (koy_strcmp(input, "help") == 0) {

            print("=====COMMANDS=====\n\n", 0x05);
            print("-----BASIC-----\n", 0x01);
            print("help - This is used to know current commands of KOY_OS.\n", 0x0F);
            print("clear - clear the screen.\n", 0x0F);
            print("version - This is used to know version of KOY_OS.\n", 0x0F);
            print("about - This is used to know about KOY_OS.\n\n", 0x0F);
            print("-----SYSTEM_CTRL-----\n", 0x01);
            print("lock - This locks the system.\n", 0x0F);
        }

        else if (koy_strcmp(input, "clear") == 0) {

            clear_screen(0x0B);
        }

        else if (koy_strcmp(input, "version") == 0) {

            print("KOY_OS Version 1.0\n", 0x0F);
        }

        else if (koy_strcmp(input, "about") == 0) {
            print("This is KOY_OS made by Dhawal Ryl at age 15.\n", 0x0F);
        }

        else if (koy_strcmp(input, "dla") == 0) {
            print("Amitabh Bacchan has 1 Crore life insurance by Policy Bazaar.\n",0x06);
        }

        else if (koy_strcmp(input, "lock") == 0) {
            lock_screen();
        }

        else if (koy_strcmp(input, "shutdown") == 0) {
            print("KOY_OS Shutting down.........\n", 0x0A);
            delay(100000000);
            shutdown();
        }
        else if (koy_strcmp(input, "") == 0) {

        }

        else {

            print("Unknown command\n", 0x04);
        }
    }
}