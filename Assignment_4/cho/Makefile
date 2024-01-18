CC = gcc
CFLAGS = -g

SRC = lib.c glomem.c
OBJ = $(SRC:.c=.o)

all: main

main: main.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o main

