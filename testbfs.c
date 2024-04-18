#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 40  // Увеличено для соответствия размерам лабиринта

typedef struct {
    int x, y;
    int from_direction;  // направление, из которого пришли в эту клетку
} Node;

char maze[MAX_SIZE][MAX_SIZE] = {
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "P         X       X   X   X   X",
    "X X XXXXX X XXXXX X X X X X X X",
    "X X X     X X   X   X X X X X X",
    "X X XXX XXX X XXXXX XXX X XXX X",
    "X X   X     X     X X   X   X X",
    "X XXX XXX XXXXX X X X XXXXX X X",
    "X   X   X X   X X X X     X X X",
    "XXX XXX XXX X X X X XXXXXXX X X",
    "X X   X     X X X X X X   X   X",
    "X X XXXXXXXXX X XXX X X X XXX X",
    "X   X     X   X       X X   X X",
    "X XXX XXX X XXXXXXXXXXX XXX X X",
    "X X   X     X           X X   X",
    "XXX XXX XXXXX XXXXXXXXXXX XXXXX",
    "X   X X   X   X   X         X X",
    "X XXX XXX X XXX X XXXXX XXX X X",
    "X X       X     X     X   X   X",
    "X XXXXXXXXXXXXXXXXXXX XXX XXX X",
    "X                         X   K",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
};

int distances[21][31];  // Общее количество шагов до каждой клетки
Node predecessors[MAX_SIZE][MAX_SIZE];  // массив предшественников

void enqueue(Node *queue, int *rear, Node node) {
    queue[(*rear)++] = node;
}

Node dequeue(Node *queue, int *front) {
    return queue[(*front)++];
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

int main() {
    printf("%c", maze[19][30]);
    int start_x = 1, start_y = 0;  // координаты 'P'
    int exit_x = 19, exit_y = 30;  // координаты 'K'
    
    bfs(start_x, start_y, exit_x, exit_y);
    print_path(start_x, start_y, exit_x, exit_y);

    return 0;
}