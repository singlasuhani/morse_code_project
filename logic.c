#include <stdio.h>
#include <stdlib.h>
#include "logic.h"  // logic header for game logic 
#include "led.h"    // led header for led control 
#include "morse.h" // morse code header for Morse code


extern int level;  // external variable for game level

// variables for level sizes and game stats
int lvl1_size; 
int lvl2_size;
int lvl3_size;
int lvl4_size;

int count_incorrect;
int count_correct;
int lives;

// Variables to store answers 
char ans_char;
const char* ans_word = {0};
char level1[256] = {0};
char level2[256] = {0};
char level3[256] = {0};
char level4[256] = {0};

void init_game() {

  count_correct = 0;     // Initialize correct count
  count_incorrect = 0;   // Initialize incorrect count
  lives = 3;             // Initialize player lives to 3
  led_init();            // Initialize led
  led_set_color(4);      // Set initial led color 

}

void clear_buffer(char* buf, int* lvlsize) {
    memset(buf, 0, strlen(buf));  // Clear the buffer
    *lvlsize = 0;                 // Reset level size
} 

//start level 1
void level1_begin() {
  led_set_color(lives);           // Set led color based on lives
  lvl1_size = 0;                   // Reset level 1 size
  // this part changes from level to level
  const char* morse_char; 
  ans_char = generate_random_character(); // Generate a random character for the player to encode
  morse_encode_char(&ans_char, &morse_char);  // Encode it to Morse code
  // Print the character and its Morse code
  printf("Character to write: %c\nMorse code of character to write: %s\n", ans_char, morse_char);

}

void level1_input(char c) {
  level1[lvl1_size] = c;   // Store input in level buffer
  lvl1_size++;             // Increment level size
  printf("%c", c);           // Print the input character
}

void level1_end() {
  char lvl1_check;
  morse_decode_char(level1, &lvl1_check); // Decode the Morse input
  int correct = lvl1_check == ans_char;    // Check if input matches the answer
  // Handle correct or incorrect input
  if (correct) {
    printf("\nCorrect!\n"); // Correct input
    count_correct++;         // Increment correct count
    if (lives < 3) {          
      lives++;  //  If lives are less than 3, increment lives
    }
    if (count_correct % 5 == 0) {   // Level up condition
      level++;                  // Increment level number
      level2_begin();    // Begin level 2
      return;
    }
  } else {                 
    printf("\nIncorrect, try again. \n");  // Incorrect input
    count_incorrect++;                     // Increment incorrect count
    lives--;                                // Decrement a life
    if (lives < 1) {                        
      game_over();                          // If lives reach 0, end the game
    } 
  }
  led_set_color(lives);       // Update LED based on remaining lives
  clear_buffer(level1, &lvl1_size);  // Clear the buffer for next input

  if (correct) { 
    level1_begin(); // If correct, start next character
  }
}

// Start level 2
void level2_begin() {
  led_set_color(lives); // Update LED color based on lives
  lvl2_size = 0;        // Reset level 2 size
  ans_char = generate_random_character(); // Generate new random character for level 2
  printf("Character to write: %c\n", ans_char);  // Prompt player with the character to encode in Morse
}

void level2_input(char c) {
  level2[lvl2_size] = c; // Store player's Morse input in buffer
  lvl2_size++;           // Increment buffer size
  printf("%c", c);        // Print the input character
}

void level2_end() {
  char lvl2_check;                 
  morse_decode_char(level2, &lvl2_check);  // Decode Morse input to character
  int correct = lvl2_check == ans_char;    // Compare decoded character with expected answer
  // Handle correct or incorrect inputs
  if (correct) {
    printf("\nCorrect!\n");         // correct input
    count_correct++;                // Increment correct count

    if (lives < 3) {
      lives++; // If lives are less than 3, increment lives
    }
    if (count_correct % 5 == 0) { // Level up condition
      level++;
      level3_begin(); // begin with level 3
      return;
    }
  } else {
    printf("\nIncorrect, try again. \n");  // incorrect input
    count_incorrect++; // Increment incorrect count
    lives--; // Decrement a life 
    if (lives < 1) { // Check for game over 
      game_over();
    }
  }

  led_set_color(lives);  // Update LED color based on remaining lives
  clear_buffer(level2, &lvl2_size);  // Clear buffer for next input

  if (correct) {
    level2_begin();  // If correct, start next character

  }
}

// Start level 3
void level3_begin() {
  led_set_color(lives);  // Set LED color based on lives
  lvl3_size = 0;         // Reset level 3 size
  char morse_word[256] = {};     // Buffer to hold Morse code of the word
  ans_word = generate_random_word();  // Generate new random word for level 3
  morse_encode_word(ans_word, morse_word);  // Encode word to Morse code
  printf("Word to write: %s\nMorse code of word to write: %s\n", ans_word, morse_word);  // Provide player with word and its Morse code
}

void level3_input(char c) {
  level3[lvl3_size] = c;   // Store input in level buffer
  lvl3_size++;            // Increment buffer size
  printf("%c", c);        // Print the input character

}

// End level 3: check player's input against correct Morse code for word
void level3_end() {
  char lvl3_check[256] = {};      // Temporary buffer to hold decoded word
  morse_decode_word(level3, lvl3_check);  // Decode Morse input
  int correct = strcmp(lvl3_check, ans_word) == 0;  // Compare decoded word with expected answer
  // Handle correct or incorrect inputs
  if (correct) {
    printf("\nCorrect!\n");         // correct input
    count_correct++;                // Increment correct count
    if (lives < 3) {                 // If lives are less than 3, increment lives
      lives++;
    }
    if (count_correct % 5 == 0) {   // level up condition
      level++;
      level4_begin();             // begin level 4
      return;
    }
  } else {
    printf("\nIncorrect, try again. \n"); // incorrect input
    count_incorrect++;                     // Increment incorrect count
    lives--;                             // Decrement a life for incorrect input
     
    if (lives < 1) {               // Check for game over 
                                    
      game_over();
    }
  }
  led_set_color(lives);  // Update LED color based on lives
  clear_buffer(level3, &lvl3_size); // Clear input buffer for next input


  if (correct) {
    level3_begin(); // If correct, start next character
  }
}

// Start level 4
void level4_begin() {
  led_set_color(lives);  // Set LED color based on lives
  lvl4_size = 0;         // Reset buffer size for level 4
  ans_word = generate_random_word();  // Generate new random word for level 4
  printf("Word to write: %s\n", ans_word);  // Prompt player with the word to encode in Morse
}

// Process player's input in level 4
void level4_input(char c) {
  level4[lvl4_size] = c;  // Store player's Morse input in buffer
  lvl4_size++;            // Increment buffer size
  printf("%c", c);        //  // Print the input character

}

void level4_end() {
  char lvl4_check[256] = {};      // Temporary buffer to hold decoded word
  morse_decode_word(level4, lvl4_check);  // Decode Morse input to word
  int correct = strcmp(lvl4_check, ans_word) == 0;  // Compare decoded word with expected answer
  // Handle correct or incorrect guesses
  if (correct) {
    printf("\nCorrect!\n");         // correct input
    count_correct++;                // Increment correct count
    if (lives < 3) {                // If lives are less than 3, increment lives
      lives++;
    }
    if (count_correct % 5 == 0) {  // Victory condition
      printf("\nYOU WON!\n");      
    }
  } else {
    printf("\nIncorrect, try again. \n"); // incorrect input
    count_incorrect++; // Increment incorrect count
    lives--; // Decrement a life for incorrect input
    if (lives < 1) {
      game_over();    // Check for game over 
  
    }
  }
  led_set_color(lives);  // Update LED color based on lives
  clear_buffer(level4, &lvl4_size);  // Clear the buffer for next input

  if (correct) {
    level4_begin(); // If correct, start next character
  }
}

// Handle game over condition
void game_over() {

  
  printf("Game over!"
          "Total guesses correct: %d"
          "Total guesses incorrect: %d \n"
          "Current Level: %d \n"
          ,count_correct, count_incorrect, level); // Display game over message and stats

  level = 0;  // Reset level to prevent further game actions
}
}
