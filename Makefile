# Определение компилятора
CC=gcc

# Определение флагов компилятора
CFLAGS=-Wall -g

# Исполняемый файл, который мы хотим создать
TARGET=program

# Список объектных файлов
OBJS=main.o bfs.o read.o wyjscie.o file_loading.o

# Правило по умолчанию
all: $(TARGET)

# Как собрать исполняемый файл
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Зависимости для объектных файлов
main.o: main.c read.h bfs.h wyjscie.h file_loading.h
	$(CC) $(CFLAGS) -c main.c

bfs.o: bfs.c bfs.h
	$(CC) $(CFLAGS) -c bfs.c

read.o: read.c read.h
	$(CC) $(CFLAGS) -c read.c

wyjscie.o: wyjscie.c wyjscie.h bfs.h
	$(CC) $(CFLAGS) -c wyjscie.c

file_loading.o: file_loading.c file_loading.h
	$(CC) $(CFLAGS) -c file_loading.c
# Правило для очистки проекта
clean:
	rm -f $(TARGET) $(OBJS)

# Правило для запуска программы с параметрами
run: $(TARGET)
	./$(TARGET) -f input_filename.txt -b input_filename.bin -o output_filename

# Альтернативное правило для запуска с возможностью передавать параметры
run_custom:
	./$(TARGET) $(ARGS)