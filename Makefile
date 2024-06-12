
CC=cc


CFLAGS=-Wall -g


TARGET=program


OBJS=main.o mbfs.o mdir.o msplit.o msplitbin.o mwyjscie.o file_loading.o


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)


main.o: main.c file_loading.h mdir.h msplit.h mbfs.h mwyjscie.h mconfig.h msplitbin.h
	$(CC) $(CFLAGS) -c main.c

mbfs.o: mbfs.c mbfs.h
	$(CC) $(CFLAGS) -c mbfs.c

mdir.o: mdir.c mdir.h
	$(CC) $(CFLAGS) -c mdir.c

msplit.o: msplit.c msplit.h
	$(CC) $(CFLAGS) -c msplit.c

msplitbin.o: msplitbin.c msplitbin.h
	$(CC) $(CFLAGS) -c msplitbin.c

mwyjscie.o: mwyjscie.c mwyjscie.h
	$(CC) $(CFLAGS) -c mwyjscie.c

file_loading.o: file_loading.c file_loading.h
	$(CC) $(CFLAGS) -c file_loading.c

clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET) -f input_filename.txt -b input_filename.bin -o output_filename

run_custom:
	./$(TARGET) $(ARGS)