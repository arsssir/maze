#include "msplit.h"

void parse_maze(const char *file_path, Position *posP, Position *posK, int *rows, int *cols, int *num_chunks) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_SIZE];
    int line_count = 0;
    int file_count = 0;
    int total_lines = 0;
    int line_length = 0;
    posP->x = posP->y = posK->x = posK->y = -1;
    FILE *outfile = NULL;

    // First, count the total number of lines and find line length
    while (fgets(line, sizeof(line), file)) {
        total_lines++;
    }

    // Reset the file pointer to the beginning of the file
    fseek(file, 0, SEEK_SET);

    while (fgets(line, sizeof(line), file)) {
        if (line_count % LINES_PER_FILE == 0) {
            if (outfile) {
                fclose(outfile);
            }
            char output_filename[64];
            snprintf(output_filename, sizeof(output_filename), "chunks/part_%d.txt", ++file_count);
            outfile = fopen(output_filename, "w");
            if (!outfile) {
                perror("Failed to open output file");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }

        if (line_length == 0) {
            line_length = strlen(line);
            if (line[line_length - 1] == '\n' || line[line_length - 1] == '\r') {
                line_length--;
            }
            if (line[line_length - 1] == '\n' || line[line_length - 1] == '\r') {
                line_length--;
            }
        }

        fprintf(outfile, "%s", line);

        // Check for 'P' and 'K' and store their coordinates
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == 'P' && posP->x == -1) {
                posP->x = line_count;
                posP->y = i;
            }
            if (line[i] == 'K' && posK->x == -1) {
                posK->x = line_count;
                posK->y = i;
            }
        }

        line_count++;
    }

    if (outfile) {
        fclose(outfile);
    }

    fclose(file);

    *rows = total_lines;
    *cols = line_length;
    *num_chunks = file_count;
    // Print total lines, line length, and coordinates of 'P' and 'K'
    printf("Общее количество чанков: %d\n", *num_chunks);
    printf("Общее количество строк: %d\n", *rows);
    printf("Длина строки: %d\n", *cols);
    printf("Координаты 'P': (%d, %d)\n", posP->x, posP->y);
    printf("Координаты 'K': (%d, %d)\n", posK->x, posK->y);
}