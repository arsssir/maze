#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "bfs.h"

const int8_t directions[4][2] = {
    {-1, 0},  // Вверх
    {0, 1},   // Вправо
    {1, 0},   // Вниз
    {0, -1}   // Влево
};

void enqueue(Node *queue, uint16_t *rear, Node node) {
    queue[(*rear)++] = node;
}

Node dequeue(Node *queue, uint16_t *front) {
    return queue[(*front)++];
}

const char* get_turn_name(int8_t last_dir, int8_t new_dir) {
    int8_t diff = (new_dir - last_dir + 4) % 4; // Обеспечиваем цикличность направлений
    switch (diff) {
        case 0:
            return "CONTINUE STRAIGHT\n";
        case 1:
            return "TURN RIGHT\n";
        case 2:
            return "TURN AROUND\n";
        case 3:
            return "TURN LEFT\n";
        default:
            return "UNKNOWN TURN\n";
    }
}

void bfs(uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, char **maze, uint16_t rows, uint16_t cols, int8_t **predecessors){
    Node queue[rows*cols];
    
    uint16_t front = 0, rear = 0;
    bool distances[rows][cols];

    for (uint16_t i = 0; i < rows; i++) {
        for (uint16_t j = 0; j < cols; j++) {
            distances[i][j] = 0;
            predecessors[i][j] = -1;
        }
    }

    distances[start_x][start_y] = 1;
    enqueue(queue, &rear, (Node){start_x, start_y, -1}); // Начальное направление не установлено

    bool flag = 0;
    while (front < rear && flag == 0) {
        Node current = dequeue(queue, &front);

        for (int8_t i = 0; i < 4; i++) {
            uint16_t nx = current.x + directions[i][0];
            uint16_t ny = current.y + directions[i][1];

            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] != 'X' && distances[nx][ny] == 0) {
                distances[nx][ny] = 1;
                enqueue(queue, &rear, (Node){nx, ny, i});
                if(nx == exit_x && ny == exit_y){
                    predecessors[exit_x][exit_y] = i;
                    flag = 1;
                } else if (current.dir != i) {
                    predecessors[current.x][current.y] = current.dir;
                }
            }
        }
    }
}

void print_path(uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, int8_t **predecessors) {
    uint16_t x = exit_x, y = exit_y;
    int8_t last_dir = predecessors[x][y];
    uint16_t steps = 0;

    if (last_dir == -1) {
        printf("No valid path or starting direction.\n");
        return;
    }

    printf("START\n");

    while (x != start_x || y != start_y) {
        int8_t pred = predecessors[x][y];

        if(pred == -1){
            pred = last_dir;
        }
        if (pred == last_dir) {
            // Продолжаем в том же направлении
            steps++;
        } else {
            // Направление изменилось, выводим пройденное расстояние и описание поворота
            if (steps > 0) {
                printf("FORWARD %d \n", steps);
                printf("%s", get_turn_name(last_dir, pred));
            }
            // Сброс счетчика шагов
            steps = 1; // начинаем отсчет заново, так как мы уже сделали шаг в новом направлении
        }
        // Обновляем 'last_dir' и перемещаемся к следующему предшественнику
        last_dir = pred;
        x -= directions[pred][0];
        y -= directions[pred][1];
    }

    
    if (steps > 0) {
        printf("FORWARD %d\n", steps);
    }
    printf("STOP at (%d, %d)\n", x, y);
}