#include "mwyjscie.h"

void txt_compress(const char *input_file, int num_letters, FILE *final_fp) {
    FILE *output_fp = fopen(input_file, "r");
    //FILE *final_fp = fopen(final_output, "w");

    if (output_fp == NULL || final_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    char prev_char = fgetc(output_fp);
    int count = 1;

    fprintf(final_fp, "START\n");

    for (int i = 0; i < num_letters; i++) {
        char ch = fgetc(output_fp);
        if (ch == prev_char) {
            count++;
        } else {
            if (count > 1) {
//                fprintf(final_fp, "%c %d\n", prev_char, count);
                fprintf(final_fp, "FOWARD %d\n", count);
            } else {
//                fprintf(final_fp, "%c %d\n", prev_char, count);
                fprintf(final_fp, "FOWARD 1\n");
            }
            if (prev_char == UP) {
                if (ch == LEFT) {
                    fprintf(final_fp, "TURNLEFT\n");
                } else if (ch == RIGHT) {
                    fprintf(final_fp, "TURNRIGHT\n");
                }
            } else if (prev_char == LEFT) {
                if (ch == UP) {
                    fprintf(final_fp, "TURNRIGHT\n");
                } else if (ch == DOWN) {
                    fprintf(final_fp, "TURNLEFT\n");
                }
            } else if (prev_char == DOWN) {
                if (ch == LEFT) {
                    fprintf(final_fp, "TURNRIGHT\n");
                } else if (ch == RIGHT) {
                    fprintf(final_fp, "TURNLEFT\n");
                }
            } else if (prev_char == RIGHT) {
                if (ch == UP) {
                    fprintf(final_fp, "TURNLEFT\n");
                } else if (ch == DOWN) {
                    fprintf(final_fp, "TURNRIGHT\n");
                }
            }
            prev_char = ch;
            count = 1;
        }
    }

    fprintf(final_fp, "STOP\n");

    // Close files
    fclose(output_fp);
}

void print_file_to_console(const char *input_file) {
    FILE *input_fp = fopen(input_file, "r");

    if (input_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    char ch;
    while ((ch = fgetc(input_fp)) != EOF) {
        printf("%c", ch);
    }

    // Close files
    fclose(input_fp);
}
