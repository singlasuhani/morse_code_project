#include <stdio.h>              // Standard input/output functions
#include <pico/stdlib.h>        // Standard library for Pico
#include <stdlib.h>             // Standard library functions
#include <math.h>               // Math functions
#include <string.h>             // String handling functions
#include "hardware/gpio.h"      // Hardware GPIO functions
#include "hardware/watchdog.h"  // Hardware watchdog functions
#include "morse.h"              // Morse code related functions
#include "logic.h"              // Game logic functions


// Declare the main assembly code entry point.
void main_asm();

// Initialise a GPIO pin – see SDK for detail on gpio_init()
void asm_gpio_init(uint pin) {
    gpio_init(pin);             
}

// Set direction of a GPIO pin – see SDK for detail on gpio_set_dir()
void asm_gpio_set_dir(uint pin, bool out) {
    gpio_set_dir(pin, out);
}

// Get the value of a GPIO pin – see SDK for detail on gpio_get()
bool asm_gpio_get(uint pin) {
    return gpio_get(pin);
}

// Set the value of a GPIO pin – see SDK for detail on gpio_put()
void asm_gpio_put(uint pin, bool value) {
    gpio_put(pin, value);
}


// Enable falling and rising edge interrupts – see SDK for detail on gpio_set_irq_enabled()
void asm_gpio_set_irq(uint pin) {
    gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_FALL, true);
}

// global var to keep track of level
int level = -1;

// void level1_begin() {
//     printf("level 1 started: ");
// }
// void level1_input(char c) {
//     printf("%c", c);
// }
// void level1_end() {
//     printf("level 1 ended\n");
// }


// void level2_begin() {
//     printf("level 2 started: ");
// }
// void level2_input(char c) {
//     printf("%c", c);
// }
// void level2_end() {
//     printf("level 2 ended\n");
// }


// void level3_begin() {
//     printf("level 3 started: ");
// }
// void level3_input(char c) {
//     printf("%c", c);
// }
// void level3_end() {
//     printf("level 3 ended\n");
// }


// void level4_begin() {
//     printf("level 4 started: ");
// }
// void level4_input(char c) {
//     printf("%c", c);
// }
// void level4_end() {
//     printf("level 4 ended\n");
// }

// space counter for morse input
int space_counter = 0;

// Array to store Morse code input
char level_input[32] = {};
// input counter for level
int level_input_counter = 0;

void clear_level_input() {
    // clear level input array
    memset(level_input, 0, sizeof(level_input));
    // reset input counter
    level_input_counter = 0;
}

//main function of the game
void handle_Dot_Dash(int state) {
    char c;
    if (state == 0) {
        // process dot
        watchdog_update(); // update watchdog timer
        space_counter = 0; // reset space counter
        c = '*';           // store dot in c
    } else if (state == 1) {
        // process dash
        watchdog_update(); // update watcdog timer
        space_counter = 0; // reset space counter
        c = '-';           // store dash in c
    } else {
        // Ignore spaces at the start to let the user 'rest'.
        // process space
        if (level_input_counter != 0) {
            space_counter++; // increment space counter
        }
        c = ' ';             // store space in c
    }

    // check if the game is in the inital stage
    if (level == -1) {
        if (c == '*') {
            // Store dot to level_input and print to screen
            level_input[level_input_counter++] = '*';
            printf("*");
        }
        else if (c == '-') {
            // Store dash to level_input and print to screen
            level_input[level_input_counter++] = '-';
            printf("-");
        }
        else {
            // Return if no space
            if (space_counter == 0) {
                return;
            }

            printf("\n"); // print newline

            char level_char;
            morse_decode_char(level_input, &level_char); // get ascii representation of morse

            clear_level_input(); // clear input
            space_counter = 0;   // reset space counter

            // check if input is invalid
            if (level_char == '?') {
                printf("Invalid input, please try again\n");
                return;
            }

            watchdog_enable(9000, 1); // enable watchdog timer

            // process chosen start level
            switch (level_char) {
                case '1':
                    level = 1;
                    level1_begin();
                    break;
                case '2':
                    level = 2;
                    level2_begin();
                    break;
                case '3':
                    level = 3;
                    level3_begin();
                    break;
                case '4':
                    level = 4;
                    level4_begin();
                    break;
            }
        }
    }

    // game logic for level 1
    else if (level == 1) {
        // check if space
        if (c == ' ') {
            // check if space was detected
            if (space_counter == 1) {
                space_counter = 0;       // reset space coutner
                level_input_counter = 0; // reset level input counter
                level1_end();            // end level 1
            }
        }
        else {
            level1_input(c);             // process level 1 input
            level_input_counter++;       // increment input counter
        }
    }
    // game logic for level 2
    else if (level == 2) {
        // check if space
        if (c == ' ') {
            // check if space was detected
            if (space_counter == 1) {
                space_counter = 0;       // reset space counter
                level_input_counter = 0; // reset level input counter
                level2_end();            // end level 2
            }
        }
        else {
            level2_input(c);             // process level 2 input
            level_input_counter++;       // increment input counter
        }
    }
    // game logic for level 3
    else if (level == 3) {
        // check if current space and 2 spaces were detected
        if (c == ' ' && space_counter == 2) {
            space_counter = 0;          // reset space coutner
            level_input_counter = 0;    // reset level input counter
            level3_end();               // end level 3
        }
        // check if not a space and input was detected
        if (c != ' ' || level_input_counter != 0) {
            level3_input(c);            // process level 2 input
            level_input_counter++;      // increment input counter
        }
    }
    // game logic for level 4
    else if (level == 4) {
        // check if current space and 2 spaces were detected
        if (c == ' ' && space_counter == 2) {
            space_counter = 0;          // reset space counter
            level_input_counter = 0;    // reset level input counter
            level4_end();               // end level 4
        }
        if (c != ' ' || level_input_counter != 0) {
            level4_input(c);            // process level 4 input
            level_input_counter++;      // incremetn input counter
        }
    }
}

// declare startup message
const char* startup_message =
" _______  _______  _______  _______  _______          \n"
"(       )(  ___  )(  ____ )(  ____ \\(  ____ \\|\\     /|\n"
"| () () || (   ) || (    )|| (    \\/| (    \\/( \\   / )\n"
"| || || || |   | || (____)|| (_____ | (__     \\ (_) / \n"
"| |(_)| || |   | ||     __)(_____  )|  __)     \\   /  \n"
"| |   | || |   | || (\\ (         ) || (         ) (   \n"
"| )   ( || (___) || ) \\ \\__/\\____) || (____/\\   | |   \n"
"|/     \\|(_______)|/   \\__/\\_______)(_______/   \\_/   \n"
"------------------------------------------------------\n"
"                       Group 30                       \n"
"------------------------------------------------------\n"
"Instructions:\n"
"- Level 1: Characters will be displayed at random WITH their morse code representation\n"
"- Level 2: Characters will be displayed at random WITHOUT their morse code representation\n"
"- Level 3: Words will be displayed at random WITH their morse code representation\n"
"- Level 4: Words will be displayed at random WITHOUT their morse code representation\n"
"- You must input the morse code for the input provided using the GP21 button\n"
"- LONG hold GP21 button for dash\n"
"- SHORT press GP21 button for dot\n"
"- If you get 5 correct you will be advanced to the next level!\n"
"\n"
"Please input level number (in morse code) to begin the game!\n";



int main() {

    stdio_init_all();        // initialise I/O

    printf(startup_message); // print start up message

    init_game();             // initialise game logic

    main_asm();              // call main asm
    
    return(0);
}
