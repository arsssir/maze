#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_SIZE 21  // Увеличено для соответствия размерам лабиринта

bool distances[MAX_SIZE][MAX_SIZE];  // Общее количество шагов до каждой клетки
int8_t predecessors[MAX_SIZE][MAX_SIZE];

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

char maze[MAX_SIZE][MAX_SIZE] = {
    "XXXXXXXXXXXXXXXXXXXXX",
    "P         X         X",
    "X XXXXXXX XXX XXXXX X",
    "X   X   X   X     X X",
    "X X X X XXX X XXX XXX",
    "X X X X X   X   X   X",
    "X X X X XXX XXX XXX X",
    "X X X X   X X X X   X",
    "X X X XXX X X X X X X",
    "X X   X   X X   X X X",
    "X XXXXX XXX XXXXX X X",
    "X   X   X   X   X X X",
    "XXX X XXX XXX X X X X",
    "X   X X   X   X   X X",
    "XXXXX XXX X XXXXXXX X",
    "X   X X X   X X     X",
    "X X X X XXXXX X XXXXX",
    "X X   X   X   X X   X",
    "X XXXXXXX X X X X X X",
    "X         X X     X K",
    "XXXXXXXXXXXXXXXXXXXXX"
};

void enqueue(Node *queue, uint16_t *rear, Node node) {
    queue[(*rear)++] = node;
}

Node dequeue(Node *queue, uint16_t *front) {
    return queue[(*front)++];
}

bool is_junction(uint16_t x, uint16_t y) {
    uint8_t count = 0;
    for (int8_t i = 0; i < 4; i++) {
        uint16_t nx = x + directions[i][0];
        uint16_t ny = y + directions[i][1];
        if (nx >= 0 && nx < MAX_SIZE && ny >= 0 && ny < MAX_SIZE && maze[nx][ny] != 'X') {
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

void bfs(uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, int8_t predecessors[MAX_SIZE][MAX_SIZE]) {
    Node queue[MAX_SIZE * MAX_SIZE];
    uint16_t front = 0, rear = 0;

    for (int8_t i = 0; i < MAX_SIZE; i++) {
        for (int8_t j = 0; j < MAX_SIZE; j++) {
            distances[i][j] = 0;
            predecessors[i][j] = -1;
        }
    }

    distances[start_x][start_y] = 0;
    enqueue(queue, &rear, (Node){start_x, start_y, -1});  // Начальное направление не установлено

    bool flag = 0;
    while (front < rear && flag == 0) {
        Node current = dequeue(queue, &front);

        for (int8_t i = 0; i < 4; i++) {
            uint16_t nx = current.x + directions[i][0];
            uint16_t ny = current.y + directions[i][1];

            if (nx >= 0 && nx < MAX_SIZE && ny >= 0 && ny < MAX_SIZE && maze[nx][ny] != 'X' && distances[nx][ny] == 0) {
                distances[nx][ny] = 1;
                enqueue(queue, &rear, (Node){nx, ny, i});
                if(nx == exit_x && ny == exit_y){
                    predecessors[exit_x][exit_y] = current.dir;
                    flag = 1;
                } else if (current.dir != i || is_junction(nx, ny )) {
                    predecessors[current.x][current.y] = current.dir;
                }
            }
        }
    }
}

void print_path(uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, int8_t predecessors[MAX_SIZE][MAX_SIZE]) {
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

/*void bfs(int start_x, int start_y, int exit_x, int exit_y) {
    Node queue[MAX_SIZE * MAX_SIZE];
    int front = 0, rear = 0;
    
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            distances[i][j] = -1;
            predecessors[i][j].dir = -1;  // Инициализация направления предшественника
        }
    }

    if(start_y == 0){
        predecessors[start_x][start_y].dir = 1;
    } else {
        predecessors[start_x][start_y].dir = 2;
    }

    distances[start_x][start_y] = 0;
    enqueue(queue, &rear, (Node){start_x, start_y, predecessors[start_y][start_x].dir});

    while (front < rear) {
        Node current = dequeue(queue, &front);
        
        for (int i = 0; i < 4; i++) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];

            if (nx >= 0 && nx < MAX_SIZE && ny >= 0 && ny < MAX_SIZE && maze[nx][ny] != 'X' && distances[nx][ny] == -1) {
                distances[nx][ny] = distances[current.x][current.y] + 1;
                if (predecessors[current.x][current.y].dir != i || is_junction(nx, ny)) {
                    enqueue(queue, &rear, (Node){nx, ny, i});
                    predecessors[nx][ny].dir = i;  // Сохраняем направление прихода
                }
            }
        }
    }
}

void print_path(int start_x, int start_y, int exit_x, int exit_y) {
    printf("START\n");
    int x = start_x, y = start_y;
    int steps = 0;
    int last_dir = predecessors[start_x][start_y].dir; // Инициализация с направления первого шага
    if (last_dir == -1) {
        printf("No valid initial direction. Check the maze setup.\n");
        return;
    }

    while (x != exit_x || y != exit_y) {
        Node pred = predecessors[x][y];
        int new_dir = pred.dir;

        if (last_dir == new_dir) {
            steps++;  // Продолжаем в том же направлении
        } else {
            if (steps > 0) {
                printf("FORWARD %d\n", steps);
            }
            if (last_dir != -1 && new_dir != -1) {
                const char* turn = get_turn_name(last_dir, new_dir);
                if (strcmp(turn, "CONTINUE STRAIGHT") != 0) {
                    printf("TURN %s\n", turn);
                }
            }
            steps = 1;  // Сброс шагов после поворота
            last_dir = new_dir;
        }

        x += directions[new_dir][0];
        y += directions[new_dir][1];
    }

    if (steps > 0) {
        printf("FORWARD %d\n", steps);
    }
    printf("STOP\n");
}

void print_path(int start_x, int start_y, int exit_x, int exit_y) {
    int x = exit_x, y = exit_y;

    printf("put:\n");
    while (!(x == start_x && y == start_y)) {
        Node pred = predecessors[x][y];
        printf("(%d, %d) pzez %d\n", x, y, pred.from_direction);
        x -= pred.x;  // обратное движение по x
        y -= pred.y;  // обратное движение по y
    }
    printf("(%d, %d) - stertа\n", start_x, start_y);
}

void bfs(int start_x, int start_y, int exit_x, int exit_y) {
    Node queue[MAX_SIZE * MAX_SIZE];
    int front = 0, rear = 0;
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 31; j++) {
            distances[i][j] = -1;  // Инициализация расстояний
        }
    }
    distances[start_x][start_y] = 0;  // Начальная точка
    enqueue(queue, &rear, (Node){start_x, start_y, -1});

    while (front < rear) {
        Node current = dequeue(queue, &front);
        int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};  // вверх, вправо, вниз, влево

        for (int i = 0; i < 4; i++) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];

            if (nx >= 0 && nx < MAX_SIZE && ny >= 0 && ny < MAX_SIZE) {  // Убедитесь, что новые координаты в пределах лабиринта
                if (maze[nx][ny] != 'X' && distances[nx][ny] == -1) {  // Проверяем, что клетка проходима и не посещена
                    distances[nx][ny] = distances[current.x][current.y] + 1;
                    predecessors[nx][ny] = (Node){directions[i][0], directions[i][1], i};
                    enqueue(queue, &rear, (Node){nx, ny, i});
                }
            }
        }
    }
    printf("ddddddddddd\n");
}
*/

int main() {
    printf("%c", maze[19][30]); 
    uint16_t start_x = 19, start_y = 20;  // координаты 'P'
    uint16_t exit_x = 1, exit_y = 0;  // координаты 'K'
    printf("Character at the exit: %c\n", maze[1][0]);
    printf("Character at the start: %c\n", maze[19][20]);
    
    bfs(start_x, start_y, exit_x, exit_y, predecessors);

    printf("Distance Table:\n");
    for (int8_t i = 0; i < MAX_SIZE; i++) {
        for (int8_t j = 0; j < MAX_SIZE; j++) {
            printf("%3d ", predecessors[i][j]);
        }
        printf("\n");
    }

    print_path(start_x, start_y, exit_x, exit_y, predecessors);

    return 0;
}