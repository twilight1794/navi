CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99

src/main: src/main.o
#src/main.o: src/main.c


clean:
	rm -rf dist
