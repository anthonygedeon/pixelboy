CC=gcc
CFLAGS=-I.

COMPILER_FLAGS = -std=c99 -g -w -Wall

all: 
	$(CC) src/main.c $(COMPILER_FLAGS) -lSDL2 -o output

clean:
	rm output
