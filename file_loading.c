
#include "file_loading.h"

// f-cja do wy�wietlania info o u�yciu programy
void print_usage(){
    printf("Usage of the programme - program_name -f [in_filename] -o [out_filename] [-h]\n");

}

// przetwarzanie argument�w za pomoc� getopta
void process_input(int argc, char *argv[], char **input_filename, char **output_filename) {
    int opt;

    // Inicjalizacja output_filename na NULL
    *output_filename = NULL;

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
        exit(2); // blad 2. Brak mozliwosci wczytania pliku
    }

    if (!file_exists(*input_filename)) {
        fprintf(stderr, "Error. Unable to open input file - %s\n", *input_filename);
        exit(2); // blad 2. Brak mozliwosci wczytania pliku
    }

    if(is_binary_file_v2(*input_filename)){
        if(!is_valid_binary_maze_format_v2(*input_filename)){
            fprintf(stderr, "Error. Invalid maze format - %s\n", *input_filename);
            exit(5); // blad 5. Nieprawidlowy format labiryntu
        }
    }
    else
    {
        if(!is_valid_input_file(*input_filename)){
            fprintf(stderr, "Error. Invalid input file format - %s\n", *input_filename);
            exit(4); // blad 4. Nieprawidlowy format pliku
        }  

        if(!is_valid_maze_format_v2(*input_filename)){
            fprintf(stderr, "Error. Invalid maze format - %s\n", *input_filename);
            exit(5); // blad 5. Nieprawidlowy format labiryntu
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

// Funkcja do testowania poprawno?ci formatu labiryntu
bool is_valid_maze_format(const char *filename, int *start_x, int *start_y, int *exit_x, int *exit_y) {
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
    
    int i = 0;
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
        for (int j = 0; j < line_length; j++) {
            char c = line[j];
            // Проверка допустимых символов
            if (c != ' ' && c != 'P' && c != 'K' && c != 'X') {
                fprintf(stderr, "Error. Invalid character '%c' in input file - %s\n", c, filename);
                fclose(file);
                return false;
            }
            // Подсчет точек входа и выхода
            if (c == 'P') {
                entry_count++;
                *start_x = j;
                *start_y = i;
            } else if (c == 'K') {
                exit_count++;
                *exit_x = j;
                *exit_y = i;
            }
        }
        i++;

        // Проверка на наличие ровно одной точки входа и одной точки выхода
        if (entry_count != 1 || exit_count != 1) {
            fprintf(stderr, "Error. Incorrect number of entry (%d) or exit (%d) points in input file - %s\n", entry_count, exit_count, filename);
            return false;
        }
    }
    fclose(file);
    return true;
}

bool is_binary_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error. Unable to open file - %s\n", filename);
        return false;
    }

    bool binary = false;
    unsigned char buffer[1024];  // Буфер для чтения данных из файла
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytesRead; i++) {
            if (buffer[i] < 0x07 || buffer[i] > 0x7E) {
                binary = true;
                break;
            }
        }
        if (binary) break;
    }

    fclose(file);
    return binary;
}