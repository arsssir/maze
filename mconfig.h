#ifndef _CONFIG_H_
#define _CONFIG_H_

#define MAX_SIZE 1030
#define LINES_PER_FILE 50

#include <stdbool.h> //true and false
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //dla getopta
#include <stdint.h>

#define WALL 'X'
#define FREE_SPACE ' '
#define UP '0'
#define DOWN '2'
#define LEFT '3'
#define RIGHT '1'
#define START 'K'
#define TARGET 'P'
#define PATH '#'

typedef struct {
    int32_t fileID;
    int8_t esc;
    int16_t col;
    int16_t row;
    int16_t Prow;
    int16_t Pcol;
    int16_t Krow;
    int16_t Kcol;
    int32_t counter;
    int32_t solution_offset;
    char separator;
    char wall;
    char path;
} MazeHeader;

#endif