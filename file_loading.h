#ifndef FILE_LOADING_H
#define FILE_LOADING_H

#include <stdbool.h> //true and false
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //dla getopta
#include <stdint.h>
#include "mconfig.h"

void print_usage();

// przetwarzanie argument�w za pomoc� getopta
void process_input(int argc, char *argv[], char **input_filename_bin, char **input_filename_txt, char **output_filename);

//sprawdzenie czy plik istnieje w systemie
bool file_exists(const char *filename);

bool file_exists_bin(const char *filename);
// Funkcja do testowania poprawno?ci formatu labiryntu
bool is_valid_maze(const char *filename);

// funkcja do sprawdzenia poprawnosci formatu labiryntu dla pliku binarnego 
bool is_valid_binary_maze(const char *filename);

#endif