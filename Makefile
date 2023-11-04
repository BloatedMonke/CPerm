CC = gcc
CFLAGS = -Wall -Wpedantic -Wextra -Iinclude/ -fpic
VPATH = include:src:
objs = combinatorics.o permutations.o
src = permutations.c combinatorics.c
headers = combinatorics.h permutations.h
TMP = .tmp

libperms.so: $(objs) $(headers)
	$(CC) -shared -o libperms.so $(TMP)/*
	make clean

combinatorics.o: combinatorics.c combinatorics.h
	if [ ! -d $(TMP) ]; \
		then mkdir $(TMP); \
	fi
	$(CC) $(CFLAGS) -c -o $(TMP)/combinatorics.o $<

permutations.o: $(src) $(headers)
	if [ ! -d $(TMP) ]; \
		then mkdir $(TMP); \
	fi
	$(CC) $(CFLAGS) -c -o $(TMP)/permutations.o $<

.PHONY: clean
clean:
	-rm -rf $(TMP)/