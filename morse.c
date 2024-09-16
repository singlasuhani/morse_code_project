
#include "morse.h"

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))

// array of words for game (dictionary)
const char* words[] = {
    "TRIANGLE", "REVIVAL", "WORTH", "BEHAVIOR",
    "PLEDGE", "OUTLINE", "DIPLOMAT", "REPTILE",
    "FADE", "CHORD", "RESPECT", "TWIST",
    "CONCESSION", "OCCUPATION", "BACON", "SATELLITE",
    "DRAWER", "POLE", "IMPERIAL", "SWEATER",
    "HABITAT", "MEAN", "DRIBBLE", "ONION",
    "COTTAGE", "PURE", "RETIREMENT", "MATHEMATICS",
    "VOLUNTEER", "HALF", "IS", "ROAR",
    "IDENTITY", "ALLOCATION", "RETIREE", "COVERAGE",
    "PERFUME", "BROKEN", "COMFORTABLE", "REQUIREMENT",
    "TRIVIAL", "EMBRACE", "WORKSHOP", "CAMERA",
    "LANDOWNER", "COLLAPSE", "OFFENDER", "IMPOUND",
    "BIN", "HIGHWAY",
    "OCEAN", "FESTIVAL", "FROZEN", "PUZZLE",
    "WHISPER", "REFRIGERATOR", "GUITAR", "MOON",
    "CAPTAIN", "SUNSET", "ECHO", "FLOWER",
    "SUGAR", "SHIVER", "FIERCE", "BUBBLE",
    "PARADISE", "THUNDER", "SPARKLE", "ECHO",
    "CRIMSON", "ILLUSION", "WHISPER", "BLOSSOM",
    "BREEZE", "BLISS", "SERENADE", "CHIME",
    "HARMONY", "SERENITY", "VELVET", "ZENITH",
    "TRANQUIL", "MYSTERY", "SOLITUDE", "CASCADE",
    "ENCHANTED", "SERENE", "LULLABY", "SILHOUETTE",
    "GRACE", "RADIANCE", "ETHEREAL", "WHIMSICAL"
};

// array of morse representation A-Z
const char* morse_code_A_Z[] =
{
    "*-", "-***", "-*-*", "-**", 
    "*", "**-*", "--*", "****", 
    "**", "*---", "-*-", "*-**", 
    "--", "-*", "---", "*--*", 
    "--*-", "*-*", "***", "-", 
    "**-", "***-", "*--", "-**-", 
    "-*--", "--**", 
};

// array of morse representation 0-9
const char* morse_code_0_9[] =
{
    "-----", "*----", "**---", "***--", "****-", "*****", "-****", "--***", "---**", "----*"
};

char generate_random_character()
{
    // get random value
    int r = rand() % 36;

    // check if letter
    if (r < 26)
    {
        // return dec value of ascii char
        return 'A' + r;
    }
    // digit
    else
    {
        // return dec value of ascii char
        return '0' + (r - 26);
    }
}

const char* generate_random_word() 
{
    // return random word from arry
    return words[rand() % 100];
}

// function to encode ascii char to morse
void morse_encode_char(char* single_char_input, const char** morse_output)
{
    // process letter
    if (single_char_input[0] >= 'A' && single_char_input[0] <= 'Z')
    {
        // return morse representation
        *morse_output = morse_code_A_Z[single_char_input[0] - 'A'];
    }
    // process digit
    else if (single_char_input[0] >= '0' && single_char_input[0] <= '9')
    {
        // return morse representation
        *morse_output = morse_code_0_9[single_char_input[0] - '0'];
    }
    // process blank
    else
    {
        // return empty char
        *morse_output = "";
    }
}

// function to decode morse to ascii char
void morse_decode_char(char* morse_input, char* single_char_output)
{
    // Handle the case where the input is a part of a word (i.e. has a space at the end)
    char morse_buf[7] = {};
    // copy input into buffer
    strncpy(morse_buf, morse_input, min(6, (int)strlen(morse_input)));
    // get first space
    char* s = strchr(morse_buf, ' ');
    // null terminate string if space is found
    if (s != 0)
    {
        *s = '\0';
    }

    // find dec value of ascii char representation
    for (int i = 0; i < 26; i++)
    {
        // compare string until match is found
        if (strcmp(morse_buf, morse_code_A_Z[i]) == 0)
        {
            // return dec value of ascii char
            *single_char_output = 'A' + i;
            return;
        }
    }

    // find dec value fo ascii digit representation
    for (int i = 0; i < 10; i++)
    {
        // compare stirng until match is found
        if (strcmp(morse_buf, morse_code_0_9[i]) == 0)
        {
            // return dec value of ascii digit
            *single_char_output = '0' + i;
            return;
        }
    }
    // else return invalid char
    *single_char_output = '?';
}

// function to encode word from ascii to morse
void morse_encode_word(char* word_input, char* morse_output)
{
    // loop through input untill end of string
    for (char* c = word_input; *c != '\0'; c++)
    {
        // concatonate string with a space if not equal to input
        if (c != word_input)
        {
            strcat(morse_output, " ");
        }
        // else decode char and add to output string
        const char* morse_char;
        morse_encode_char(c, &morse_char);
        strcat(morse_output, morse_char);
    }
}

// function to decode word from morse to ascii
void morse_decode_word(char* morse_input, char* word_output)
{
    int i = 0;
    // loop through input
    for (; *morse_input != '\0'; morse_input = strchr(morse_input, ' ') + 1) 
    {
        // end loop if 1 is detected 
        if (morse_input == 1)
        {
            break;
        }
        // decode char
        char single_char;
        morse_decode_char(morse_input, &single_char);
        word_output[i++] = single_char;
    }
}