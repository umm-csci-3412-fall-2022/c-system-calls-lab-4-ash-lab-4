#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

// Read data from the input file, writing the same data without the vowels to the output
void disemvowel(FILE* input, FILE* output);

int main(int argc, char** argv) {
    FILE* inputFile = stdin;
    FILE* outputFile = stdout;

    // If we have been passed a file as an argument, we will attempt to open it
    if (argc > 1) {
        inputFile = fopen(argv[1], "r");

        if (!inputFile) {
            fprintf(stderr, "unable to open file '%s'\n", argv[1]);
            return 1;
        }
    }
    if (argc > 2) {
        outputFile = fopen(argv[2], "w");

        if (!outputFile) {
            fprintf(stderr, "unable to open file '%s'\n", argv[2]);
            return 1;
        }
    }

    disemvowel(inputFile, outputFile);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}


void disemvowel(FILE* input, FILE* output) {
    int read;
    // Iterate over each character of the input string
    do {
        char in;
        read = fread(&in, 1, 1, input);

        // If it is a vowel, go to the next character
        switch (in) {
            case 'a': case 'A':
            case 'e': case 'E':
            case 'i': case 'I':
            case 'o': case 'O':
            case 'u': case 'U':
            break;
            // Otherwise, move the character to the output
            default:
                fwrite(&in, 1, 1, output);
                break;
        }
    }
    while (read);
}