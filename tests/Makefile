CC = gcc
VPATH = ../src:../include
CFLAGS = -I../include
DBFLAGS = $(CFLAGS) -g3 -fsanitize=address
objects = permutations.o combinatorics.o main.o
dbobjects = dbpermutations.o dbcombinatorics.o dbmain.o
headers = permutations.h combinatorics.h

main: $(objects) $(headers)
	$(CC) -o main $(objects)

dbmain: $(dbobjects) $(headers)
	$(CC) $(DBFLAGS) -o dbmain $(dbobjects)

permutations.o: permutations.c $(headers)
combinatorics.o: combinatorics.c combinatorics.h
dbpermutations.o: permutations.c $(headers)
	$(CC) $(DBFLAGS) -c -o dbpermutations.o $<
dbcombinatorics.o: combinatorics.c combinatorics.h
	$(CC) $(DBFLAGS) -c -o dbcombinatorics.o $<
dbmain.o: main.c $(headers)
	$(CC) $(DBFLAGS) -c -o dbmain.o $<

clean:
	rm $(objects) main $(dbobjects) dbmain