CC = gcc
CFLAGS = -W
TARGET=file

all : file.o
    $(CC) $(CFAGS) $< -o $(TARGET)

file.o : file.c
    $(CC) -c $<

clean:
    rm -f *.o
    rm -f $(TARGET)
