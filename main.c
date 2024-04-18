#include <stdio.h>
#include <stdlib.h>
#include "file_loading.h" 
#include "bfs.h"
#include "read.h"
#include "wyjscie.h"

int8_t** allocate_2D_array_int8(uint16_t rows, uint16_t cols) {
    int8_t** array = malloc(rows * sizeof(int8_t*));
    for (uint16_t i = 0; i < rows; i++) {
        array[i] = malloc(cols * sizeof(int8_t));
    }
    return array;
}

int main(int argc, char *argv[]) {
    char *input_filename_txt = NULL;
    char *input_filename_bin = NULL;
    char *output_filename = NULL;
    //FILE *output_file = fopen("maze.txt", "w");
    //FILE *output_f = fopen("da.txt", "w");

    
    int output_type = 0;
    process_input(argc, argv, &input_filename_bin, &input_filename_txt, &output_filename);
    
    char filename[256];

    if (!output_filename) {
        output_type = 0;
    } else {
        strcpy(filename, output_filename);
        printf("Enter output type (1 for text file, 2 for binary file, 3 for maze in bin file): ");
        scanf("%d", &output_type);
    }

    if (input_filename_txt && input_filename_bin) {
        printf("Otwarte dwa pliki. Proszę otworzyć tylko jeden.\n");
        exit(EXIT_FAILURE);
    }

    Maze maze;
    if (input_filename_txt) {
        maze = read_maze_from_file(input_filename_txt);
    } else if (input_filename_bin) {
        maze = read_maze_from_binary_file(input_filename_bin);
    } else {
        fprintf(stderr, "Nie podano pliku labiryntu.\n");
        exit(EXIT_FAILURE);
    }

    int8_t **predecessors = allocate_2D_array_int8(maze.height, maze.width);
    if (!predecessors) {
        fprintf(stderr, "Nie udało się zaalokować pamięci dla poprzedników.\n");
        return EXIT_FAILURE;
    }

    printf("Wymiary labiryntu: %dx%d\n", maze.height, maze.width);
    printf("Pozycja startowa: (%c)\n", maze.data[maze.start.x][maze.start.y]);
    printf("Pozycja końcowa: (%c)\n", maze.data[maze.finish.x][maze.finish.y]);

    bfs(maze.start.x, maze.start.y, maze.finish.x, maze.finish.y, maze.data, maze.height, maze.width, predecessors);
    
    /*for (uint16_t i = 0; i < maze.height; i++) {
        for (uint16_t j = 0; j < maze.width; j++) {
            fprintf(output_f, "%c ", maze.data[i][j]);
        }
        fprintf(output_f, "\n");
    }

    printf("maze:\n");
    for (uint16_t i = 0; i < maze.height; i++) {
        for (uint16_t j = 0; j < maze.width; j++) {
            fprintf(output_file, "%d ", predecessors[i][j]);
        }
        fprintf(output_file, "\n");
    }*/
    
    char long_filename[260];
    if (output_type == 1) {
        snprintf(long_filename, sizeof(long_filename), "%s.txt", filename);  // Формирование имени файла с расширением .txt
        print_path_to_file(long_filename, maze.start.x, maze.start.y, maze.finish.x, maze.finish.y, predecessors);
    } else if (output_type == 2) {
        snprintf(long_filename, sizeof(long_filename), "%s.bin", filename);  // Формирование имени файла с расширением .bin
        print_path_to_binary_file(long_filename, maze.start.x, maze.start.y, maze.finish.x, maze.finish.y, predecessors);
    } else if (output_type == 3) {
        snprintf(long_filename, sizeof(long_filename), "%s.bin", filename);  // Формирование имени файла с расширением .bin
        write_maze_to_binary_file(long_filename, maze.data, maze.height, maze.width);
    } else {
        // Вывод в консоль
        print_path(maze.start.x, maze.start.y, maze.finish.x, maze.finish.y, predecessors);
    }

    for (int i = 0; i < maze.height; i++) {
        free(maze.data[i]);
    }
    free(maze.data);
    free(predecessors);

    return EXIT_SUCCESS;
}