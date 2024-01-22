CC = gcc
CFLAGS = -g

SRC = lib.c glomem.c utilM.c
OBJ = $(SRC:.c=.o)
TARGET = utilM

all: main

main: main.o $(OBJ)
    $(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lrt

main.o: $(SRC)
    $(CC) $(CFLAGS) -c $^

clean:
    rm -f *.o main