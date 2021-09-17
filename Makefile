CC=gcc
CFLAGS=-g -ansi -Wall -pedantic-errors

all: the3

the3: the3.c
	$(CC) $(CFLAGS) the3.c -o the3

clean:
	rm the3