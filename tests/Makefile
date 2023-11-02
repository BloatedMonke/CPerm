CC = gcc
VPATH = ../src:../include
CFLAGS = -I../include
objects = permutations.o combinatorics.o main.o
headers = permutations.h combinatorics.h

main: $(objects) $(headers)
	$(CC) -o main $(objects)

permutations.o: permutations.c $(headers)
combinatorics.o: combinatorics.c combinatorics.h

clean:
	rm $(objects) main