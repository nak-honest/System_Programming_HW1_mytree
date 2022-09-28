TARGET=test
OBJ=test.o get_file_info.o
CC=gcc
CFLAGS=-c

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

.c.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -rf $(OBJ)

