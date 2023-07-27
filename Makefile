CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: program

program: ml.o main.o
	$(CC) $(CFLAGS) ml.o main.o -o run -lm

ml.o: ml.c
	$(CC) $(CFLAGS) -c ml.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o run
