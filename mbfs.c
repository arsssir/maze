#include "mbfs.h"

Queue* create_queue(int capacity) {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    queue->data = (Node*)malloc(sizeof(Node) * capacity);
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = capacity;
    return queue;
}

void enqueue(Queue *queue, int row, int col) {
    if (queue->size == queue->capacity) return;
    queue->data[queue->rear] = (Node){row, col};
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size++;
}

Node dequeue(Queue *queue) {
    Node node = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return node;
}

bool is_queue_empty(Queue *queue) {
    return queue->size == 0;
}

void free_maze(char** maze, int rows) {
    for (int i = 0; i < rows; i++) {
        free(maze[i]);  // Освободить память для каждой строки
    }
    free(maze);  // Освободить память для самого массива
}

// Прототипы вспомогательных функций
char** read_maze(const char* filepath, int rows, int cols) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Error opening file.\n");
        return NULL;
    }

    char** maze = malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        maze[i] = malloc((cols + 2) * sizeof(char));
        for (int j = 0; j < 1; j++) {
            maze[i][j] = '\0';
        }
        fgets(maze[i], cols + 2, file);
    }

    fclose(file);
    return maze;
}

char find_and_return_char_from_chunk(int *loaded_chunk_number, int chunk_row_counter, int cols, int current_col, int current_row, char ***current_chunk, int how_many_chunks){
    if ((current_row <= (*loaded_chunk_number * chunk_row_counter) - 1) && (current_row >= (*loaded_chunk_number - 1) * chunk_row_counter)) {
        return (*current_chunk)[current_row - (*loaded_chunk_number - 1) * chunk_row_counter][current_col];
    }
    char new_path[25];
    snprintf(new_path, 25, "chunks/part_%d.txt", *loaded_chunk_number);
    FILE* file = fopen(new_path, "w");
    if (!file) {
        printf("Error opening file.\n");
        exit;
    }
    for (int i = 0; i < chunk_row_counter; i++) {
        fprintf(file, "%s", (*current_chunk)[i]);
    }
    fclose(file);

    free_maze(*current_chunk, chunk_row_counter);

    *loaded_chunk_number = current_row / chunk_row_counter + 1;

    snprintf(new_path, 25, "chunks/part_%d.txt", *loaded_chunk_number);

    (*current_chunk) = read_maze(new_path, chunk_row_counter, cols);

    return (*current_chunk)[current_row - (*loaded_chunk_number - 1) * chunk_row_counter][current_col];
}

void backtrack_path(char*** current_chunk, int rows, int cols, int current_row, int current_col, int* loaded_chunk_number, int chunk_row_counter, int how_many_chunks)
{
    int number_of_steps = 0;
    FILE* file = fopen("tmp/steps.txt", "w");

    while (find_and_return_char_from_chunk(loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, current_chunk, how_many_chunks) != START) {
        number_of_steps+=1;
        char step = find_and_return_char_from_chunk(loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, current_chunk, how_many_chunks);
        if (!file) {
            printf("Error opening file.\n");
            return;
        }
        fprintf(file, "%c", step);

        if (step == LEFT)
        {
            current_col -= 1;
        }
        else if (step == RIGHT)
        {
            current_col += 1;
        }
        else if (step == UP)
        {
            current_row -= 1;
        }
        else if (step == DOWN)
        {
            current_row += 1;
        }

        char new_char = find_and_return_char_from_chunk(loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, current_chunk, how_many_chunks);

        if (current_row < 0 || current_row >= rows || current_col < 0 || current_col >= cols) {
            printf("Error: Path goes out of bounds.\n");
            break;
        }
        else if (new_char == WALL) {
            printf("Error: Path goes through a wall.\n");
            break;
        }
        else if (new_char == PATH) {
            printf("Error: Path goes through itself.\n");
            break;
        }
        else if (new_char == START) {
            printf("(%d, %d)\n", current_row, current_col);
            break;
        }
    }

    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    FILE* count_file = fopen("tmp/steps_count.txt", "w");
    if (!count_file) {
        printf("Error opening file.\n");
        return;
    }
    fprintf(count_file, "%d", number_of_steps);

    fclose(file);
    fclose(count_file);
}

//void backtrack_path(char ***current_chunk, int rows, int cols, int current_row, int current_col, int *loaded_chunk_number, int chunk_row_counter, int how_many_chunks);

void bfs(int rows, int cols, int start_row, int start_col, int16_t chunk_row_counter, int how_many_chunks)
{
    bool *loaded_chunks = malloc(how_many_chunks * sizeof(bool));
    for (int i = 0; i < how_many_chunks; i++) {
        loaded_chunks[i] = false;
    }
    
    int loaded_chunk_number = start_row / chunk_row_counter + 1;

    //int start_row_in_chunk = start_row - (loaded_chunk_number - 1) * chunk_row_counter;
    char name_of_chunk[25];
    snprintf(name_of_chunk, 25, "chunks/part_%d.txt", loaded_chunk_number);
    char** current_chunk = read_maze(name_of_chunk, chunk_row_counter, cols);
    if (current_chunk == NULL) {
        printf("Error. Unable to read maze.\n");
        return;
    }

    char found = 0;
    Queue *queue = create_queue(rows * cols);
    enqueue(queue, start_row, start_col);

    while (!is_queue_empty(queue) && found == 0) {
        Node current_node = dequeue(queue);
        int current_row = current_node.row;
        int current_col = current_node.col;

        // current_row and current_col == TARGET
        if (find_and_return_char_from_chunk(&loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, &current_chunk, how_many_chunks) == TARGET) {
            found = 1;
            backtrack_path(&current_chunk, rows, cols, current_row, current_col, &loaded_chunk_number, chunk_row_counter, how_many_chunks);
            break;  // Stop the algorithm if the target is reached
        }

        // Check and update four possible directions
        int direction_offsets[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        char direction_marks[4] = {DOWN, UP, RIGHT, LEFT};
        
        for (int i = 0; i < 4; i++) {
            int neighbor_row = current_row + direction_offsets[i][0];
            int neighbor_col = current_col + direction_offsets[i][1];

            if (neighbor_row < 0 || neighbor_col < 0 || neighbor_row >= rows || neighbor_col >= cols) continue;
            char neighbor_char = find_and_return_char_from_chunk(&loaded_chunk_number, chunk_row_counter, cols, neighbor_col, neighbor_row, &current_chunk, how_many_chunks);
            if (neighbor_row >= 0 && neighbor_row < rows && neighbor_col >= 0 && neighbor_col < cols && (neighbor_char == FREE_SPACE || neighbor_char == TARGET))
            {
                if (neighbor_char == TARGET) {
                    found = 1;
                    current_chunk[neighbor_row - (loaded_chunk_number - 1) * chunk_row_counter][neighbor_col] = direction_marks[i];
                    backtrack_path(&current_chunk, rows, cols, neighbor_row, neighbor_col, &loaded_chunk_number, chunk_row_counter, how_many_chunks);
                    break;
                } else {
                    // mark the direction you came from into the neighbor cell
                    current_chunk[neighbor_row - (loaded_chunk_number - 1) * chunk_row_counter][neighbor_col] = direction_marks[i];
                    enqueue(queue, neighbor_row, neighbor_col);
                }
            }
        }
        if(loaded_chunks[loaded_chunk_number - 1] == false)
        {
            loaded_chunks[loaded_chunk_number - 1] = true;
            printf("Loaded chunk %d/%d\n", loaded_chunk_number, how_many_chunks);
        }
    }

    free(queue->data);
    free(queue);
    free_maze(current_chunk, chunk_row_counter);
    free(loaded_chunks);
}