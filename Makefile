CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: program

program: ml.o opener.o main.o
	$(CC) $(CFLAGS) ml.o opener.o main.o -o run -lm

ml.o: ml.c
	$(CC) $(CFLAGS) -c ml.c

opener.o: opener.c
	$(CC) $(CFLAGS) -c opener.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o run
