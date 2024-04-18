#ifndef READ_H
#define READ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //true and false
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //dla getopta
#include <stdint.h>

typedef struct {
    uint16_t x, y;
} Position;

typedef struct {
    char** data;
    uint16_t width;
    uint16_t height;
    Position start;
    Position finish;
} Maze;

Maze read_maze_from_file(const char* filename);

Maze read_maze_from_binary_file(const char* filename);

#endif