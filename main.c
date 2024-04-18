#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 1024
#include "file_loading.h" 

int main(int argc, char *argv[]) {
    char *input_filename = NULL;
    char *output_filename = NULL;
    int exit_x;
    int exit_y;
    int start_y;
    int start_x;
    process_input(argc, argv, &input_filename, &output_filename);


    // mo?na doda?  znalezienie drogi przez labirynt

    printf("Labirynt jest poprawny i gotowy do dalszych operacji.\n");

    free(input_filename);
    free(output_filename);

    return EXIT_SUCCESS;
}

typedef struct {
    int x, y;
    int steps;  // Шаги с последнего поворота или начала
    int dir;    // Направление движения: 0-вверх, 1-вправо, 2-вниз, 3-влево
} Node;

char maze[MAX_SIZE][MAX_SIZE];
int distances[MAX_SIZE][MAX_SIZE];  // Общее количество шагов до каждой клетки

void enqueue(Node *queue, int *rear, Node node) {
    queue[(*rear)++] = node;
}

Node dequeue(Node *queue, int *front) {
    return queue[(*front)++];
}

void bfs(int start_x, int start_y, int exit_x, int exit_y) {
    Node queue[MAX_SIZE * MAX_SIZE];
    int front = 0, rear = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            if (maze[i][j] != 'X') {
                distances[i][j] = -1; // Инициализация только проходимых клеток
            }
        }
    }

    enqueue(queue, &rear, (Node){start_x, start_y, 0, -1});
    distances[start_x][start_y] = 0;

    while (front < rear) {
        Node current = dequeue(queue, &front);
        if (current.x == exit_x && current.y == exit_y) {
            printf("Выход достигнут через %d шагов\n", distances[current.x][current.y]);
            return;
        }

        int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (int i = 0; i < 4; i++) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];
            if (nx >= 0 && nx < MAX_SIZE && ny >= 0 && ny < MAX_SIZE && maze[nx][ny] != 'X' && (distances[nx][ny] == -1 || distances[nx][ny] > distances[current.x][current.y] + 1)) {
                distances[nx][ny] = distances[current.x][current.y] + 1;
                enqueue(queue, &rear, (Node){nx, ny, (current.dir == i ? current.steps + 1 : 1), i});
                if (current.dir != i && current.dir != -1) {
                    printf("Идти %d шагов и повернуть на %s\n", current.steps, (i == 0 ? "вверх" : i == 1 ? "вправо" : i == 2 ? "вниз" : "влево"));
                }
            }
        }
    }

    printf("Выход не найден\n");
}

int main() {
    // Предполагаем, что лабиринт уже загружен в maze
    bfs(0, 0, 10, 10);
    return 0;
}