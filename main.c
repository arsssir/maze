#include "file_loading.h" 

int main(int argc, char *argv[]) {
    char *input_filename = NULL;
    char *output_filename = NULL;
    process_input(argc, argv, &input_filename, &output_filename);


    // mo?na doda?  znalezienie drogi przez labirynt

    printf("Labirynt jest poprawny i gotowy do dalszych operacji.\n");

    free(input_filename);
    free(output_filename);

    return EXIT_SUCCESS;
}
