#include "wyjscie.h"
#include "bfs.h"

// Функция для вывода пути в текстовый файл
void print_path_to_file(const char* filename, uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, int8_t **predecessors) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Cannot open file to write: %s\n", filename);
        return;
    }

    uint16_t x = exit_x, y = exit_y;
    int8_t last_dir = predecessors[x][y];
    uint16_t steps = 0;

    if (last_dir == -1) {
        fprintf(file, "No valid path or starting direction.\n");
        return;
    }

    fprintf(file, "START\n");

    while (x != start_x || y != start_y) {
        int8_t pred = predecessors[x][y];

        if(pred == -1){
            pred = last_dir;
        }
        if (pred == last_dir) {
            steps++;
        } else {
            if (steps > 0) {
                fprintf(file, "FORWARD %d\n", steps);
                fprintf(file, "%s", get_turn_name(last_dir, pred));
            }
            steps = 1;
        }
        last_dir = pred;
        x -= directions[pred][0];
        y -= directions[pred][1];
    }

    if (steps > 0) {
        fprintf(file, "FORWARD %d\n", steps);
    }
    fprintf(file, "STOP at (%d, %d)\n", x, y);

    fclose(file);
}

// Функция для вывода пути в бинарный файл
void print_path_to_binary_file(const char *filename, uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, int8_t **predecessors) {
    FILE *file = fopen(filename, "wb");  // Открываем файл для бинарной записи
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    // Записываем стартовое сообщение
    char *start_step = "START\n";
    fwrite(start_step, sizeof(char), strlen(start_step), file);

    uint16_t x = exit_x, y = exit_y;
    int8_t last_dir = predecessors[x][y];
    uint16_t steps = 0;
    char forward[50];

    if (last_dir == -1) {
        fclose(file);
        return;
    }

    // Обратный путь от выхода к входу, записывая шаги и повороты
    while (x != start_x || y != start_y) {
        int8_t pred = predecessors[x][y];

        if (pred == -1) pred = last_dir;

        if (pred == last_dir) {
            steps++;
        } else {
            if (steps > 0) {
                snprintf(forward, sizeof(forward), "FORWARD %d\n", steps);
                fwrite(forward, sizeof(char), strlen(forward), file);
            }
            snprintf(forward, sizeof(forward), "%s", get_turn_name(last_dir, pred));
            fwrite(forward, sizeof(char), strlen(forward), file);
            last_dir = pred;
            steps = 1; // начинаем отсчет заново после поворота
        }

        x -= directions[pred][0];
        y -= directions[pred][1];
    }

    if (steps > 0) {
        sprintf(forward, "FORWARD %d\n", steps);
        fwrite(forward, sizeof(char), strlen(forward), file);
    }

    // Записываем завершающее сообщение
    char *stop_step = "STOP\n";
    fwrite(stop_step, sizeof(char), strlen(stop_step), file);

    fclose(file);  // Закрываем файл
}

void write_maze_to_binary_file(const char *filename, char** mazeData, uint16_t height, uint16_t width) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    // Write the height and width as 16-bit unsigned integers
    fwrite(&height, sizeof(uint16_t), 1, file);
    fwrite(&width, sizeof(uint16_t), 1, file);

    // Write the maze data
    for (int i = 0; i < height; ++i) {
        fwrite(mazeData[i], sizeof(char), width, file);
    }

    fclose(file);
}