#pragma once

// Generates a random alphanumeric character
char generate_random_character();

// Generates a random word 
const char* generate_random_word();

// Encodes a single character into Morse code
void morse_encode_char(char* single_char_input, const char** morse_output);

// Decodes a Morse code string into a single character
void morse_decode_char(char* morse_input, char* single_char_output);

// Encodes the word into Morse code
void morse_encode_word(char* word_input, char* morse_output);

// Decodes a Morse code into word
void morse_decode_word(char* morse_input, char* word_output);
