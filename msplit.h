#ifndef _SPLIT_H_
#define _SPLIT_H_

#include "mconfig.h"

typedef struct {
    int x, y; // Позиции символов в лабиринте
} Position;

void parse_maze_bin(const char *file_path, Position *posP, Position *posK, int *rows, int *cols, int *num_chunks);

void parse_maze(const char *file_path, Position *posP, Position *posK, int *rows, int *cols, int *num_chunks); 

#endif