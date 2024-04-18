#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 2050
//#include "file_loading.h" 
#include <stdbool.h>
#include <stdbool.h> //true and false
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //dla getopta
#include <stdint.h>
#include "bfs.h"

typedef struct {
    uint16_t x, y;
} Position;

typedef struct {
    char** data;
    uint16_t width;
    uint16_t height;
    Position start;
    Position finish;
} Maze;

void process_input(int argc, char *argv[], char **input_filename, char **output_filename) {
    int opt;

    while ((opt = getopt(argc, argv, "f:o:h")) != -1) {
        switch (opt) {
            case 'f':
                *input_filename = optarg;
                break;
            case 'o':
                *output_filename = optarg;
                break;
            case 'h':
                //print_usage();
                exit(EXIT_SUCCESS);
            default:
                //print_usage();
                exit(EXIT_FAILURE);
        }
    }
}

Maze read_maze_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[MAX_SIZE];
    char** mazeData = NULL;
    uint16_t linesCount = 0;
    uint16_t maxWidth = 0;
    uint16_t capacity = 10;  // Начальная емкость массива

    mazeData = malloc(sizeof(char*) * capacity);
    if (!mazeData) { // Проверка на успешное выделение памяти
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        uint16_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0'; // Удаляем символ новой строки
            len--;
        }
        if (len > maxWidth) maxWidth = len;

        if (linesCount >= capacity) {
            capacity *= 2; // Удваиваем размер массива
            char** temp = realloc(mazeData, sizeof(char*) * capacity);
            if (!temp) {
                fprintf(stderr, "Memory reallocation failed\n");
                // Освобождаем выделенную память перед выходом
                for (uint16_t i = 0; i < linesCount; i++) free(mazeData[i]);
                free(mazeData);
                fclose(file);
                exit(EXIT_FAILURE);
            }
            mazeData = temp;
        }

        mazeData[linesCount] = strdup(line); // Дублируем строку
        if (!mazeData[linesCount]) { // Проверка на успешное выделение памяти
            fprintf(stderr, "Memory allocation for line failed\n");
            // Освобождаем выделенную память перед выходом
            for (uint16_t i = 0; i < linesCount; i++) free(mazeData[i]);
            free(mazeData);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        linesCount++;
    }

    fclose(file); // Закрываем файл

    Position start = {0, 0}, finish = {0, 0};
    for (uint16_t i = 0; i < linesCount; i++) {
        for (uint16_t j = 0; mazeData[i][j] != '\0'; j++) {
            if (mazeData[i][j] == 'K') {
                start.x = i;
                start.y = j;
            } else if (mazeData[i][j] == 'P') {
                finish.x = i;
                finish.y = j;
            }
        }
    }

    // Уменьшаем размер массива данных до фактически используемого
    char** temp = realloc(mazeData, sizeof(char*) * linesCount);
    if (!temp) {
        fprintf(stderr, "Memory reallocation failed\n");
        for (uint16_t i = 0; i < linesCount; i++) free(mazeData[i]);
        free(mazeData);
        exit(EXIT_FAILURE);
    }
    mazeData = temp;

    Maze maze = {mazeData, maxWidth, linesCount, start, finish};
    return maze;
}

int8_t** allocate_2D_array_int8(uint16_t rows, uint16_t cols) {
    int8_t** array = malloc(rows * sizeof(int8_t*));
    for (uint16_t i = 0; i < rows; i++) {
        array[i] = malloc(cols * sizeof(int8_t));
    }
    return array;
}

int main(int argc, char *argv[]) {
    char *input_filename = NULL;
    char *output_filename = NULL;
    process_input(argc, argv, &input_filename, &output_filename);
    Maze maze = read_maze_from_file(input_filename);
    int8_t **predecessors = allocate_2D_array_int8(maze.height, maze.width);
    if (!predecessors) {
        fprintf(stderr, "Failed to allocate memory for predecessors.\n");
        return EXIT_FAILURE;
    }
    
    /*printf("Distance Table:\n");
    for (int8_t i = 0; i < maze.height; i++) {
        for (int8_t j = 0; j < maze.width; j++) {
            printf("%3d ", predecessors[i][j]);
        }
        printf("\n");
    }*/

    printf("Maze dimensions: %dx%d\n", maze.height, maze.width);
    printf("Start position: (%d, %d)\n", maze.start.x, maze.start.y);
    printf("finish position: (%d, %d)\n", maze.finish.x, maze.finish.y);
    
    /*printf("maze:\n");
    for (int8_t i = 0; i < maze.height; i++) {
        for (int8_t j = 0; j < maze.width; j++) {
            printf("%c ", maze.data[i][j]);
        }
        printf("\n");
    }*/

    bfs(maze.start.x, maze.start.y, maze.finish.x, maze.finish.y, maze.data, maze.height, maze.width, predecessors);
    
    printf("turns Table:\n");
    for (int8_t i = 0; i < maze.height; i++) {
        for (int8_t j = 0; j < maze.width; j++) {
            printf("%3d ", predecessors[i][j]);
        }
        printf("\n");
    }

    print_path(maze.start.x, maze.start.y, maze.finish.x, maze.finish.y, predecessors);
    // Освобождение памяти
    for (int i = 0; i < maze.height; i++) {
        free(maze.data[i]);
    }
    free(maze.data);

    // mo?na doda?  znalezienie drogi przez labirynt

    printf("Labirynt jest poprawny i gotowy do dalszych operacji.\n");

    //free(input_filename);
    //free(output_filename);

    return EXIT_SUCCESS;
}