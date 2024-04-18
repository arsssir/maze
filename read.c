#include "read.h"

#define MAX_SIZE 1030

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
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[len - 1] = '\0';
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

Maze read_maze_from_binary_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open binary file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    Maze maze;
    fread(&maze.height, sizeof(uint16_t), 1, file);
    fread(&maze.width, sizeof(uint16_t), 1, file);

    maze.data = malloc(maze.height * sizeof(char*));
    if (!maze.data) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char lineBuffer[MAX_SIZE + 1];  // Assuming MAX_SIZE is defined elsewhere
    maze.start.x = maze.start.y = maze.finish.x = maze.finish.y = 0;  // Initialize positions

    for (uint16_t i = 0; i < maze.height; i++) {
        if (fread(lineBuffer, sizeof(char), maze.width, file) != maze.width) {
            fprintf(stderr, "Failed to read a line from file: %s\n", filename);
            for (uint16_t j = 0; j < i; j++) free(maze.data[j]);
            free(maze.data);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        lineBuffer[maze.width] = '\0';  // Null-terminate the string

        maze.data[i] = strdup(lineBuffer);
        if (!maze.data[i]) {
            fprintf(stderr, "Memory allocation for line failed\n");
            for (uint16_t j = 0; j < i; j++) free(maze.data[j]);
            free(maze.data);
            fclose(file);
            exit(EXIT_FAILURE);
        }

        // Search for 'K' and 'P' to set start and finish positions
        for (uint16_t j = 0; j < maze.width; j++) {
            if (maze.data[i][j] == 'K') {
                maze.start.x = i;
                maze.start.y = j;
            } else if (maze.data[i][j] == 'P') {
                maze.finish.x = i;
                maze.finish.y = j;
            }
        }
    }

    fclose(file);
    return maze;
}