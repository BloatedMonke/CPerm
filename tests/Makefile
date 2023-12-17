VPATH = ../include ../src
CFLAGS = -I../include -I../src -Wall -Wpedantic -Wextra -Werror -std=c99 -g3 -fsanitize=address
SRC = main.c permutations.c combinatorics.c
objs = ${SRC:.c=.o}
CC = gcc
TEST_FILE_LEN = 3
AD_FLAG = -DTEST_FILE_LEN=$(TEST_FILE_LEN)
STD_FILES = P44char.txt P44int.txt
.DEFAULT_GOAL = all

# test all optimisation levels
.PHONY: all clean test

debug: $(objs) $(SRC)
	$(CC) $(CFLAGS) -O0 -o $@ $(objs)
debug_O1: $(objs) $(SRC)
	$(CC) $(CFLAGS) -O1 -o $@ $(objs)
debug_O2: $(objs) $(SRC)
	$(CC) $(CFLAGS) -O2 -o $@ $(objs)
debug_O3: $(objs) $(SRC)
	$(CC) $(CFLAGS) -O3 -o $@ $(objs)
clean:
	rm *.o
all: debug debug_O1 debug_O2 debug_O3

test: all
	rm *.o
	touch test_results.txt
	@echo "no optimisations" >> test_results.txt
	@echo ""                 >> test_results.txt
	./debug    $(STD_FILES)  >> test_results.txt
	@echo ""                 >> test_results.txt
	@echo "level 1"          >> test_results.txt
	@echo ""                 >> test_results.txt
	./debug_O1 $(STD_FILES)  >> test_results.txt
	@echo ""                 >> test_results.txt
	@echo "level 2"          >> test_results.txt
	@echo ""                 >> test_results.txt
	./debug_O2 $(STD_FILES)  >> test_results.txt
	@echo ""                 >> test_results.txt
	@echo "level 3"          >> test_results.txt
	@echo ""                 >> test_results.txt
	./debug_O3 $(STD_FILES)  >> test_results.txt
	rm debug*
	less -R test_results.txt

permutations.o: permutations.c
combinatorics.o: combinatorics.c

main.o: main.c
	$(CC) $(CFLAGS) $(AD_FLAG) -o $@ -c $<
