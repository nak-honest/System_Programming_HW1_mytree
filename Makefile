TARGET=mytree
OBJ=print_my_tree.o get_file_info.o get_directory_info.o main.o
CC=gcc
CFLAGS=-c

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

.c.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -rf $(OBJ)
