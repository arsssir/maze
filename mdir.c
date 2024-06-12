#include "mdir.h"

void delete_directory_content(const char *path) {
    DIR *d = opendir(path);
    if (d) {
        struct dirent *p;
        while ((p = readdir(d))) {
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }
            char file_path[257];
            snprintf(file_path, sizeof(file_path), "%s/%s", path, p->d_name);
            if (remove(file_path) == -1) {
                perror("Error deleting file");
            }
        }
        closedir(d);
    }
}

// Функция для создания каталога
void create_directory(const char *path) {
    if (mkdir(path, 0777) == -1) {
        if (errno != EEXIST) {
            perror("Error creating directory");
            exit(EXIT_FAILURE);
        }
    }
}

void if_dir(const char *dir_name){
    struct stat st;
    if (stat(dir_name, &st) == -1) {
        // Каталог не существует, создаем его
        create_directory(dir_name);
        printf("Directory created successfully\n");
    } else if (S_ISDIR(st.st_mode)) {
        // Каталог существует, удаляем его содержимое
        delete_directory_content(dir_name);
        printf("Directory content deleted successfully\n");
    } else {
        // Файл с таким именем уже существует, но это не каталог
        fprintf(stderr, "%s already exists and is not a directory\n", dir_name);
        exit(EXIT_FAILURE);
    }
}