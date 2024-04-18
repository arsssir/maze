#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //true and false
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //dla getopta
#include <stdint.h>

#define MAX_SIZE 2050  // Максимальный размер строки

// Структура для хранения позиций в лабиринте
typedef struct {
    int x, y;
} Position;

// Структура для хранения данных лабиринта
typedef struct {
    char **data;
    int width, height;
    Position start;
    Position finish;
} Maze;

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
    if (!mazeData) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        uint16_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';  // Удаляем символ новой строки
        }

        if (len > maxWidth) {
            maxWidth = len;
        }

        if (linesCount >= capacity) {
            capacity *= 2;
            char** temp = realloc(mazeData, sizeof(char*) * capacity);
            if (!temp) {
                fprintf(stderr, "Memory reallocation failed\n");
                for (uint16_t i = 0; i < linesCount; i++) free(mazeData[i]);
                free(mazeData);
                fclose(file);
                exit(EXIT_FAILURE);
            }
            mazeData = temp;
        }

        mazeData[linesCount] = strdup(line);
        if (!mazeData[linesCount]) {
            fprintf(stderr, "Memory allocation for line failed\n");
            for (uint16_t i = 0; i < linesCount; i++) free(mazeData[i]);
            free(mazeData);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        linesCount++;
    }

    fclose(file);
    
    printf("maze:\n");
    for (uint16_t i = 0; i < linesCount; i++) {
        printf("%s\n", mazeData[i]);
    }

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
        fprintf(stderr, "Final memory reallocation failed\n");
        for (uint16_t i = 0; i < linesCount; i++) free(mazeData[i]);
        free(mazeData);
        exit(EXIT_FAILURE);
    }
    mazeData = temp;

    Maze maze = {mazeData, maxWidth, linesCount, start, finish};
    return maze;
}
// Функция освобождения памяти лабиринта
void free_maze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        free(maze->data[i]);
    }
    free(maze->data);
}

bool distances[1025][1025];

typedef struct {
    uint16_t x, y;
    int8_t dir;
} Node;

const int8_t directions[4][2] = {
    {0, 1},  // Вправо
    {1, 0},  // Вниз
    {0, -1}, // Влево
    {-1, 0}  // Вверх
};

void bfs(uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, char **maze, uint16_t rows, uint16_t cols, int8_t **predecessors){
    Node *queue = malloc(sizeof(Node) * rows * cols);
    if (!queue) {
        fprintf(stderr, "Failed to allocate memory for queue\n");
        return;
    }
    
    uint16_t front = 0, rear = 0;
    
    bool **distances = malloc(rows * sizeof(bool *));
for (uint16_t i = 0; i < rows; i++) {
    distances[i] = malloc(cols * sizeof(bool));
    for (uint16_t j = 0; j < cols; j++) {
        distances[i][j] = 0;
    }
}

    for (uint16_t i = 0; i < rows; i++) {
        for (uint16_t j = 0; j < cols; j++) {
            predecessors[i][j] = -1;
        }
    }

    

    distances[start_x][start_y] = 1;
    queue[rear++] = (Node){start_x, start_y, -1};  // Начальное направление не установлено

    bool flag = 0;
    while (front < rear && flag == 0) {
        Node current = queue[front++];

        for (int8_t i = 0; i < 4; i++) {
            uint16_t nx = current.x + directions[i][0];
            uint16_t ny = current.y + directions[i][1];

            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] != 'X' && distances[nx][ny] == 0) {
                distances[nx][ny] = 1;
                queue[rear++] = (Node){nx, ny, i};
                if(nx == exit_x && ny == exit_y){
                    predecessors[exit_x][exit_y] = i;
                    flag = 1;
                } else if (current.dir != i) {
                    predecessors[current.x][current.y] = current.dir;
                }
            }
        }
    }
    free(queue);
}

int8_t** allocate_2D_array_int8(uint16_t rows, uint16_t cols) {
    int8_t** array = malloc(rows * sizeof(int8_t*));
    for (uint16_t i = 0; i < rows; i++) {
        array[i] = malloc(cols * sizeof(int8_t));
    }
    return array;
}

int main(int argc, char **argv) {
    /*if (argc < 2) {
        printf("Usage: %s <maze_file>\n", argv[0]);
        return EXIT_FAILURE;
    }*/
    FILE *output_file = fopen("maze.txt", "w");
    FILE *output_f = fopen("da.txt", "w");

    const char *filename = argv[1];
    Maze maze = read_maze_from_file("maze1.txt");
    
    int8_t **predecessors = allocate_2D_array_int8(maze.height, maze.width);
    if (!predecessors) {
        fprintf(stderr, "Nie udało się zaalokować pamięci dla poprzedników.\n");
        return EXIT_FAILURE;
    }
    
    bfs(maze.start.x, maze.start.y, maze.finish.x, maze.finish.y, maze.data, maze.height, maze.width, predecessors);

    for (uint16_t i = 0; i < maze.height; i++) {
        for (uint16_t j = 0; j < maze.width; j++) {
            fprintf(output_f, "%c ", maze.data[i][j]);
        }
        fprintf(output_f, "\n");
    }

    printf("maze:\n");
    for (uint16_t i = 0; i < maze.height; i++) {
        for (uint16_t j = 0; j < maze.width; j++) {
            fprintf(output_file, "%d ", distances[i][j]);
        }
        fprintf(output_file, "\n");
    }

    
    // Освобождаем память
    free_maze(&maze);
    return 0;
}