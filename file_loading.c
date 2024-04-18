
#include "file_loading.h"

// f-cja do wy�wietlania info o u�yciu programy
void print_usage(){
    printf("Usage of the programme - program_name -f [in_filename] -o [out_filename] [-h]\n");

}

// przetwarzanie argument�w za pomoc� getopta
void process_input(int argc, char *argv[], char **input_filename, char **output_filename) {
    int opt;

    while ((opt = getopt(argc, argv, "f:o:h")) != -1) {
        switch (opt) {
            case 'f':
                *input_filename = optarg;
                break;
            case 'o':
                *output_filename = optarg;
                break;
            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
            default:
                print_usage();
                exit(EXIT_FAILURE);
        }
    }

    if (*input_filename == NULL){
        print_usage();
        exit(2); // B��d 2. Brak mo�liwo�ci wczytania pliku
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

// Funkcja do testowania poprawno?ci formatu labiryntu
bool is_valid_maze_format(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error. Unable to open input file - %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[MAX_SIZE]; 

    // liczb? punkt�w wej?cia i wyj?cia
    uint8_t entry_count = 0;  // Счетчик точек входа
    uint8_t exit_count = 0;

    //szeroko?? labiryntu (liczba kolumn)
    int maze_width = -1; // -1 tzn ?e jeszcze nie sprawdzono szeroko?ci

    while (fgets(line, sizeof(line), file)) {
        int line_length = strlen(line);
        //usun?? znak nowej linii, je?li istnieje
        if (line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
            line_length--;
        }

        //pierwsza linia musi ma t? sam? d?ugo?? co wszystkie kolejne
        if (maze_width == -1) {
            maze_width = line_length;
        } else if (maze_width != line_length) {
            fprintf(stderr, "Error. Inconsistent maze width in input file - %s\n", filename);
            fclose(file);
            return false;
        }

        // czy linia sk?ada si? tylko z dozwolonych znak�w
        for (int i = 0; i < line_length; i++) {
            char c = line[i];
            // Проверка допустимых символов
            if (c != ' ' && c != 'P' && c != 'K' && c != 'X') {
                fprintf(stderr, "Error. Invalid character '%c' in input file - %s\n", c, filename);
                fclose(file);
                return false;
            }
            // Подсчет точек входа и выхода
            if (c == 'P') {
                entry_count++;
            } else if (c == 'K') {
                exit_count++;
            }
        }

        fclose(file);

        // Проверка на наличие ровно одной точки входа и одной точки выхода
        if (entry_count != 1 || exit_count != 1) {
            fprintf(stderr, "Error. Incorrect number of entry (%d) or exit (%d) points in input file - %s\n", entry_count, exit_count, filename);
            return false;
        }

    return true;
    
    }
}
