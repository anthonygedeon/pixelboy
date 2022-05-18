CC=gcc
CFLAGS=-I.

COMPILER_FLAGS = -std=c99 -g -w -Wall

all: 
	$(CC) src/main.c $(COMPILER_FLAGS) -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o output

clean:
	rm output
