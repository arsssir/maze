#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

//#define MAX_SIZE 21  // Увеличено для соответствия размерам лабиринта
  // Общее количество шагов до каждой клетки


typedef struct {
    uint16_t x, y;      // координаты узла
    int8_t dir;
           // направление движения после последнего изменения
} Node;

const int8_t directions[4][2] = {
    {-1, 0},  // Вверх
    {0, 1},   // Вправо
    {1, 0},   // Вниз
    {0, -1}   // Влево
};

//char **maze;

void enqueue(Node *queue, uint16_t *rear, Node node) {
    queue[(*rear)++] = node;
}

Node dequeue(Node *queue, uint16_t *front) {
    return queue[(*front)++];
}

bool is_junction(uint16_t x, uint16_t y, char** maze, uint16_t rows, uint16_t cols) {
    uint8_t count = 0;
    for (int8_t i = 0; i < 4; i++) {
        uint16_t nx = x + directions[i][0];
        uint16_t ny = y + directions[i][1];
        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] != 'X') {
            count++;
        }
    }
    return count > 2;
}

const char* get_turn_name(int8_t last_dir, int8_t new_dir) {
    int8_t diff = (new_dir - last_dir + 4) % 4; // Обеспечиваем цикличность направлений
    switch (diff) {
        case 0:
            return "CONTINUE STRAIGHT";
        case 1:
            return "TURN RIGHT";
        case 2:
            return "TURN AROUND";
        case 3:
            return "TURN LEFT";
        default:
            return "UNKNOWN TURN";
    }
}

void bfs(uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, char **maze, uint16_t rows, uint16_t cols, int8_t **predecessors){
    Node queue[rows * cols];
    uint16_t front = 0, rear = 0;
    bool distances[rows][cols];

    for (int8_t i = 0; i < rows; i++) {
        for (int8_t j = 0; j < cols; j++) {
            distances[i][j] = 0;
            predecessors[i][j] = -1;
        }
    }

    distances[start_x][start_y] = 1;
    enqueue(queue, &rear, (Node){start_x, start_y, -1});  // Начальное направление не установлено

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
                printf("%s\n", get_turn_name(last_dir, pred));
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

char** allocate_2D_array_char(uint16_t rows, uint16_t cols) {
    char** array = malloc(rows * sizeof(char*));
    for (uint16_t i = 0; i < rows; i++) {
        array[i] = malloc(cols * sizeof(char));
    }
    return array;
}


int8_t** allocate_2D_array_int8(uint16_t rows, uint16_t cols) {
    int8_t** array = malloc(rows * sizeof(int8_t*));
    for (uint16_t i = 0; i < rows; i++) {
        array[i] = malloc(cols * sizeof(int8_t));
    }
    return array;
}

int main() {
    //printf("%c", maze[19][30]); 
    uint16_t rows = 21, cols = 21;
    char** maze = allocate_2D_array_char(rows, cols);

    // Fill the maze for demonstration purposes (normally you would read this from a file)
 const char* example_maze[21] = {
    "XXXXXXXXXXXXXXXXXXXXX",
    "P       X   X       X",
    "X XXXXX XXX X XXX X X",
    "X X   X   X   X   X X",
    "X XXX XXX X XXX XXXXX",
    "X   X   X X   X     X",
    "XXX XXX XXXXX XXXXX X",
    "X X   X   X   X     X",
    "X XXX XXX X XXX XXXXX",
    "X   X   X X   X     X",
    "X X XXX X XXX XXXXX X",
    "X X X   X X   X     X",
    "X XXX XXX X XXX XXXXX",
    "X   X X   X X X     X",
    "X X X X XXX X XXXXX X",
    "X X   X           X X",
    "X XXXXXXXXXXXXXXXXX X",
    "X   X     X     X   X",
    "XXX X XXX X XXX X X X",
    "X     X     X     X K",
    "XXXXXXXXXXXXXXXXXXXXX"
};
    for (uint16_t i = 0; i < rows; i++) {
        strncpy(maze[i], example_maze[i], cols);
    }
    
    int8_t **predecessors = allocate_2D_array_int8(rows, cols);
    uint16_t start_x = 19, start_y = 20;  // координаты 'P'
    uint16_t exit_x = 1, exit_y = 0;  // координаты 'K'
    printf("Character at the exit: %c\n", maze[1][0]);
    printf("Character at the start: %c\n", maze[19][20]);
    
    bfs(start_x, start_y, exit_x, exit_y, maze, rows, cols, predecessors);

    printf("Distance Table:\n");
    for (int8_t i = 0; i < rows; i++) {
        for (int8_t j = 0; j < cols; j++) {
            printf("%3d ", predecessors[i][j]);
        }
        printf("\n");
    }

    print_path(start_x, start_y, exit_x, exit_y, predecessors);

    return 0;
}