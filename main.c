#include <string.h>

#include "file_loading.h" 
#include "mdir.h"
#include "msplit.h"
#include "mbfs.h"
#include "mwyjscie.h"
#include "mconfig.h"
#include "msplitbin.h"

void print_from_offset(FILE *file, int offset){
    fseek(file, offset, SEEK_SET);
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        printf("%c", ch);
    }
}

int main(int argc, char *argv[]) {
    const char *dir_name = "chunks";
    const char *tmp_name = "tmp";
    const char *res_name = "results";

    if_dir(dir_name);
    if_dir(tmp_name);
    if_dir(res_name);

    char *input_filename_txt = NULL;
    char *input_filename_bin = NULL;
    char *output_filename = NULL;

    int output_type = 0;
    process_input(argc, argv, &input_filename_bin, &input_filename_txt, &output_filename);
    
    char filename[256];

    if (input_filename_txt && input_filename_bin) {
        printf("Otwarte dwa pliki. Proszę otworzyć tylko jeden.\n");
        exit(EXIT_FAILURE);
    }

    //const char* input_filename = argv[1];  // замените на путь к вашему файлу
    //int chunk_size = 15;  // например, разделяем на чанки по 10 строк
    Position posP, posK;
    int rows, cols, num_chunks;
    //split_maze_into_files(file_path, &posP, &posK);

    if (input_filename_txt) {
        parse_maze(input_filename_txt, &posP, &posK, &rows, &cols, &num_chunks);
    } else if (input_filename_bin) {
        MazeHeader inf;
        inf = read_bin_file(input_filename_bin);
        parse_maze("tmp/bin_to_txt.txt", &posP, &posK, &rows, &cols, &num_chunks);
    } else {
        fprintf(stderr, "Nie podano pliku labiryntu.\n");
        exit(EXIT_FAILURE);
    }

    bfs(rows, cols, posK.x, posK.y, LINES_PER_FILE, num_chunks);

    int steps;
    FILE *input_file = fopen("tmp/steps_count.txt", "r");
    if (input_file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    fscanf(input_file, "%d", &steps);

    //txt_compress("tmp/steps.txt", steps, "tmp/final_output.txt");
    if (!output_filename) {
        output_type = 0;
    } else {
        strcpy(filename, output_filename);
        printf("Enter output type (1 for text file, 2 for binary file): ");
        scanf("%d", &output_type);
    }
    char long_filename[270];
    if (output_type == 1) {
        snprintf(long_filename, sizeof(long_filename), "results/%s.txt", filename);
        FILE *final_out = fopen(long_filename, "w");  // Формирование имени файла с расширением .txt
        txt_compress("tmp/steps.txt", steps, final_out);
        fclose(final_out);
    } else if (output_type == 2) {
        snprintf(long_filename, sizeof(long_filename), "results/%s.bin", filename);
        FILE *final_out_bin = fopen(long_filename, "wb");  // Формирование имени файла с расширением .bin
        txt_compress("tmp/steps.txt", steps, final_out_bin);
        fclose(final_out_bin);
    } else {
        // Вывод в консоль
        txt_compress("tmp/steps.txt", steps, stdout);
    }

        /*if (output_filename == NULL) {
            print_file_to_console("tmp/final_output.txt");
    }*/

    fclose(input_file);
    return 0;
}