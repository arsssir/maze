
#include "file_loading.h"

// f-cja do wy�wietlania info o u�yciu programy
void print_usage(){
    printf("Użycie programu: nazwa_programu -f [nazwa_pliku.txt] -b [nazwa_pliku.bin] -o [nazwa_pliku_wyjściowego] [-h]\n");

}

// przetwarzanie argument�w za pomoc� getopta
void process_input(int argc, char *argv[], char **input_filename_bin, char **input_filename_txt, char **output_filename) {
    int opt;

    while ((opt = getopt(argc, argv, "f:o:b:h")) != -1) {
        switch (opt) {
            case 'f':
                *input_filename_txt = optarg;
                break;
            case 'o':
                *output_filename = optarg;
                break;
            case 'b':
                *input_filename_bin = optarg;
                break;
            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
            default:
                print_usage();
                exit(EXIT_FAILURE);
        }
    }

    if (*input_filename_bin == NULL && *input_filename_txt == NULL){
        print_usage();
        exit(2); // blad 2. Brak mozliwosci wczytania pliku
    } else if(*input_filename_txt != NULL) {
        if (!file_exists(*input_filename_txt)) {
            fprintf(stderr, "Error. Unable to open input file - %s\n", *input_filename_txt);
            exit(2); // blad 2. Brak mozliwosci wczytania pliku
        } else {
            if(!is_valid_maze(*input_filename_txt)){
                fprintf(stderr, "Error. Invalid maze format - %s\n", *input_filename_txt);
               exit(5); // blad 5. Nieprawidlowy format labiryntu
            }
        }
    } else if(*input_filename_bin != NULL) {
        if (!file_exists_bin(*input_filename_bin)) {
            fprintf(stderr, "Error. Unable to open input file - %s\n", *input_filename_bin);
            exit(2); // blad 2. Brak mozliwosci wczytania pliku
        } else {
            if(!is_valid_binary_maze(*input_filename_bin)){
                fprintf(stderr, "Error. Invalid maze format - %s\n", *input_filename_bin);
               exit(5); // blad 5. Nieprawidlowy format labiryntu
            }
        }
    }
}

//sprawdzenie czy plik istnieje w systemie
bool file_exists(const char *filename){
    FILE *file = fopen(filename, "r");
    if(file){
        fclose(file);
        return true;
    }
    return false;
}

bool file_exists_bin(const char *filename) {
    FILE *file = fopen(filename, "rb");  // Open the file in binary read mode
    if (file) {
        fclose(file);  // If the file is opened successfully, close it
        return true;   // Return true to indicate the file exists
    }
    return false;  // If the file could not be opened, return false
}

// Funkcja do testowania poprawno?ci formatu labiryntu
bool is_valid_maze(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error. Unable to open input file - %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[MAX_SIZE];
    uint8_t entry_count = 0, exit_count = 0;
    int maze_width = -1, i = 0;
    
    while (fgets(line, sizeof(line), file)) {
        int line_length = strlen(line);

        while (line_length > 0 && (line[line_length - 1] == '\n' || line[line_length - 1] == '\r')) {
        line[line_length - 1] = '\0';
        line_length--;
    }

        if (maze_width == -1) {
            maze_width = line_length;
        } else if (maze_width != line_length) {
            fprintf(stderr, "Error. Inconsistent maze width in input file - %s\n", filename);
            fclose(file);
            return false;
        }

        for (int j = 0; j < line_length; j++) {
            char c = line[j];
            if (c != ' ' && c != 'P' && c != 'K' && c != 'X') {
                fprintf(stderr, "Error. Invalid character '%c' in input file - %s\n", c, filename);
                fclose(file);
                return false;
            }
            if (c == 'P') entry_count++;
            if (c == 'K') exit_count++;
        }
        i++;
    }
    if (entry_count != 1 || exit_count != 1) {
        fprintf(stderr, "Error. Incorrect number of entry (%d) or exit (%d) points in input file - %s\n", entry_count, exit_count, filename);
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

// funkcja do sprawdzenia poprawnosci formatu labiryntu dla pliku binarnego 
bool is_valid_binary_maze(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error. Unable to open file - %s\n", filename);
        return false;
    }

    uint16_t height, width;
    fread(&height, sizeof(uint16_t), 1, file);
    fread(&width, sizeof(uint16_t), 1, file);

    uint8_t entry_count = 0, exit_count = 0;

    char *line = malloc(width + 1); // +1 for the null terminator
    if (!line) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return false;
    }

    for (uint16_t i = 0; i < height; i++) {
        if (fread(line, sizeof(char), width, file) != width) {
            fprintf(stderr, "Failed to read a line from file: %s\n", filename);
            free(line);
            fclose(file);
            return false;
        }
        line[width] = '\0'; // Null-terminate the string

        for (uint16_t j = 0; j < width; j++) {
            char c = line[j];
            if (c != 'X' && c != ' ' && c != 'P' && c != 'K') {
                fprintf(stderr, "Error. Invalid character '%c' in maze file on line %u.\n", c, i + 1);
                free(line);
                fclose(file);
                return false;
            }
            if (c == 'P') entry_count++;
            if (c == 'K') exit_count++;
        }
    }

    free(line);
    fclose(file);

    if (entry_count != 1 || exit_count != 1) {
        fprintf(stderr, "Error. Incorrect number of entry (%d) or exit (%d) points in binary maze file.\n", entry_count, exit_count);
        return false;
    }

    return true;
}