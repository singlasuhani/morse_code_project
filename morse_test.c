#include <stdio.h>

void morse_test() {
    printf("Morse test\n");  // Start the Morse code test

     // Variables for Morse code and characters
    char* morse_output;
    char single_char_output;
    char* single_char_input = "A";  // Test character 'A'
    char* morse_input = "*-";       // Morse code for 'A'

    // Convert 'A' to Morse code
    morse_encode_char(single_char_input, &morse_output);
    printf("Morse encode char: %s\n", morse_output);

    // Convert '*-' back to 'A'
    morse_decode_char(morse_input, &single_char_output);
    printf("Morse decode char: %c\n", single_char_output);

    // Test word 'HELLO'
    char* word_input = "HELLO";
    char morse_output_word[100] = {};

    // Convert 'HELLO' to Morse code
    morse_encode_word(word_input, morse_output_word);
    printf("Morse encode word: %s\n", morse_output_word);

    char word_output[100] = {};
    morse_decode_word(morse_output_word, word_output);

    // Convert Morse code back to 'HELLO'
    printf("Morse decode word: %s\n", word_output);

    // Morse code test done
    printf("Morse test complete\n");
}