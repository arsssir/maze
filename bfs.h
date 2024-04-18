#ifndef BFS1_H
#define BFS1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#define MAX_SIZE 1030

typedef struct {
    uint16_t x, y;      // координаты узла
    int8_t dir;
} Node;

extern const int8_t directions[4][2];

void enqueue(Node *queue, uint16_t *rear, Node node);

Node dequeue(Node *queue, uint16_t *front);

bool is_junction(uint16_t x, uint16_t y, char** maze, uint16_t rows, uint16_t cols);

const char* get_turn_name(int8_t last_dir, int8_t new_dir);

void bfs(uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, char **maze, uint16_t rows, uint16_t cols, int8_t **predecessors);

void print_path(uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, int8_t **predecessors);

#endif