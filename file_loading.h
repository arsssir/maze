#ifndef FILE_LOADING_H
#define FILE_LOADING_H

#define MAX_SIZE 1024

#include <stdbool.h> //true and false
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //dla getopta
#include <stdint.h>

void print_usage();

void process_input(int argc, char *argv[], char **input_filename, char **output_filename);

bool file_exists(const char *filename);

bool is_valid_maze_format(const char *filename); 

#endif
