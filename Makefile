TARGET=mytree
OBJ=print_my_tree_v2.o get_file_info.o sort_tree.o mytree.o
CC=gcc
CFLAGS=-c

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

.c.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -rf $(OBJ)
