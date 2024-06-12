#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mconfig.h"

void write_code_world_to_file(FILE *file, unsigned char sym, unsigned char how_many, int col, unsigned int *current_col, int Prow, int Pcol, int Krow, int Kcol, unsigned int *current_row);

MazeHeader read_bin_file(char *filepath);