#include "msplitbin.h"

void write_code_world_to_file(FILE *file, unsigned char sym, unsigned char how_many, int col, unsigned int *current_col, int Prow, int Pcol, int Krow, int Kcol, unsigned int *current_row) {
    for (int i = 0; i < how_many + 1; i++) {
        if ((*current_row == Pcol) && (*current_col == Prow-1)) {
            fwrite("P", sizeof(char), 1, file);
            (*current_col)++;
        } else if ((*current_row == Kcol) && (*current_col == Krow-1)){
            fwrite("K", sizeof(char), 1, file);
            (*current_col)++;
        }
        else {
            fwrite(&sym, sizeof(char), 1, file);
            (*current_col)++;
        }
        if (*current_col == col) {
            fwrite("\n", sizeof(char), 1, file);
            *current_col = 0;
            (*current_row)++;
        }
    }
}

MazeHeader read_bin_file(char *filepath) {

    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        printf("Error: File not found.\n");
        exit(EXIT_FAILURE);
    }

    MazeHeader maze;

    fread(&maze.fileID, sizeof(int32_t), 1, file);
    fread(&maze.esc, sizeof(int8_t), 1, file);
    fread(&maze.col, sizeof(int16_t), 1, file);
    fread(&maze.row, sizeof(int16_t), 1, file);
    fread(&maze.Prow, sizeof(int16_t), 1, file);
    fread(&maze.Pcol, sizeof(int16_t), 1, file);
    fread(&maze.Krow, sizeof(int16_t), 1, file);
    fread(&maze.Kcol, sizeof(int16_t), 1, file);
    fseek(file, 12, SEEK_CUR);
    fread(&maze.counter, sizeof(int32_t), 1, file);
    fread(&maze.solution_offset, sizeof(int32_t), 1, file);
    fread(&maze.separator, sizeof(char), 1, file);
    fread(&maze.wall, sizeof(char), 1, file);
    fread(&maze.path, sizeof(char), 1, file);

    printf("File ID: %d\n", maze.fileID);
    printf("ESC: %d\n", maze.esc);
    printf("Col: %d\n", maze.col);
    printf("Row: %d\n", maze.row);
    printf("Entry X: %d\n", maze.Prow);
    printf("Entry Y: %d\n", maze.Pcol);
    printf("Exit X : %d\n", maze.Krow);
    printf("Exit Y: %d\n", maze.Kcol);
    printf("Counter: %d\n", maze.counter);
    printf("Solution offset: %d\n", maze.solution_offset);
    printf("Separator: %d\n", maze.separator);
    printf("Wall: %d\n", maze.wall);
    printf("Path: %d\n", maze.path);

    FILE *file_output = fopen("tmp/bin_to_txt.txt", "w");
    unsigned int current_col = 0;
    unsigned int current_row = 1;

    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * 3);
        if (buffer == NULL) {
        printf("Error: Memory allocation failed.\n");
        fclose(file); 
        exit(EXIT_FAILURE);
        }

        for (int i = 0; i < maze.counter; i++) {
            fread(buffer, sizeof(unsigned char), 3, file);
            if (buffer[0] == maze.separator && (buffer[1] == maze.wall || buffer[1] == maze.path)) {
            write_code_world_to_file(file_output, buffer[1], buffer[2], maze.col, &current_col, maze.Prow, maze.Pcol, maze.Krow, maze.Kcol, &current_row);
         } 
        else {
        printf("Error: Input file is incorrect. Terminating program.\n");
        exit(EXIT_FAILURE);
        }
        }

    free(buffer); // Zwolnienie bufora po zako?czeniu p?tli

    fclose(file);
    fclose(file_output);
    return maze;
}