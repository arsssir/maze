#ifndef _BFS_H_
#define _BFS_H_

#include "mconfig.h"

typedef struct {
    int row, col;
} Node;

typedef struct {
    Node *data;
    int front, rear, size;
    int capacity;
} Queue;

Queue* create_queue(int capacity);

void enqueue(Queue *queue, int row, int col);

Node dequeue(Queue *queue);

bool is_queue_empty(Queue *queue);

void free_maze(char** maze, int rows);

char** read_maze(const char* filepath, int rows, int cols);

char find_and_return_char_from_chunk(int *loaded_chunk_number, int chunk_row_counter, int cols, int current_col, int current_row, char ***current_chunk, int how_many_chunks);

void backtrack_path(char*** current_chunk, int rows, int cols, int current_row, int current_col, int* loaded_chunk_number, int chunk_row_counter, int how_many_chunks);

void bfs(int rows, int cols, int start_row, int start_col, int16_t chunk_row_counter, int how_many_chunks);

#endif