#ifndef WYJSCIE_H
#define WYJSCIE_H
#include <stdio.h>
#include <stdint.h>

void print_path_to_file(const char* filename, uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, int8_t **predecessors);

void print_path_to_binary_file(const char* filename, uint16_t start_x, uint16_t start_y, uint16_t exit_x, uint16_t exit_y, int8_t **predecessors);

void write_maze_to_binary_file(const char* filename, char** mazeData,  uint16_t height, uint16_t width);

#endif