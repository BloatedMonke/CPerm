VPATH = ../include ../src
CFLAGS = -I../include -I../src -Wall -Wpedantic -Wextra -Werror -std=c99 -g3 -fsanitize=address
SRC = main.c cperm.c
OBJS0 = ${SRC:.c=O0.o}
OBJS1 = ${SRC:.c=O1.o}
OBJS2 = ${SRC:.c=O2.o}
OBJS3 = ${SRC:.c=O3.o}
HEADERS = cperm.h combinatorics.h
CC = gcc
AD_FLAG = -DMAX_TEST_FILE_COUNT=$(MAX_TEST_FILE_COUNT)
STD_FILES = P44char.txt P44int.txt P43int.txt P42int.txt P41int.txt P40int.txt \
            C44int.txt C43int.txt C42char.txt C40int.txt 
MAX_TEST_FILE_COUNT = $(words $(STD_FILES))
.DEFAULT_GOAL = all

# test all optimisation levels
.PHONY: all clean test test_clean

debugO0: $(OBJS0) $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -O0 -o $@ $(OBJS0)
debugO1: $(OBJS1) $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -O1 -o $@ $(OBJS1)
debugO2: $(OBJS2) $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -O2 -o $@ $(OBJS2)
debugO3: $(OBJS3) $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -O3 -o $@ $(OBJS3)
clean:
	rm *.o
test_clean:
	rm *.o
	rm debug*
	rm test_results.txt
all: debugO0 debugO1 debugO2 debugO3

test: all
	> test_results.txt
	@echo "no optimisations" >> test_results.txt
	@echo ""                 >> test_results.txt
	./debugO0 $(STD_FILES)  >> test_results.txt
	@echo ""                 >> test_results.txt
	@echo "level 1"          >> test_results.txt
	@echo ""                 >> test_results.txt
	./debugO1 $(STD_FILES)  >> test_results.txt
	@echo ""                 >> test_results.txt
	@echo "level 2"          >> test_results.txt
	@echo ""                 >> test_results.txt
	./debugO2 $(STD_FILES)  >> test_results.txt
	@echo ""                 >> test_results.txt
	@echo "level 3"          >> test_results.txt
	@echo ""                 >> test_results.txt
	./debugO3 $(STD_FILES)  >> test_results.txt
	less -R test_results.txt

cpermO0.o: cperm.c $(HEADERS)
	$(CC) $(CFLAGS) -O0 -o $@ -c $<
cpermO1.o: cperm.c $(HEADERS)
	$(CC) $(CFLAGS) -O1 -o $@ -c $<
cpermO2.o: cperm.c $(HEADERS)
	$(CC) $(CFLAGS) -O2 -o $@ -c $<
cpermO3.o: cperm.c $(HEADERS)
	$(CC) $(CFLAGS) -O3 -o $@ -c $<

mainO0.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) $(AD_FLAG) -O0 -o $@ -c $<
mainO1.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) $(AD_FLAG) -O1 -o $@ -c $<
mainO2.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) $(AD_FLAG) -O2 -o $@ -c $<
mainO3.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) $(AD_FLAG) -O3 -o $@ -c $<
