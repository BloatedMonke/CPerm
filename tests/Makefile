CC        := gcc
SRC       := main.c cperm.c
VPATH     := ../include ../src
ADFLAG     = -DMAX_TEST_FILE_COUNT=$(MAX_TEST_FILE_COUNT)
CFLAGS     = $(INCLUDES) $(WARNINGS) $(DB_FLAGS)
HEADERS   := cperm.h combinatorics.h
INCLUDES  := ${VPATH:%=-I%}
WARNINGS  := -Wall -Wpedantic -Wextra -Werror -std=c99
DB_FLAGS  := -g3 -fsanitize=address
STD_FILES := P44char.txt P44int.txt P43int.txt P42int.txt P41int.txt P40int.txt \
            C44int.txt C43int.txt C42char.txt C40int.txt 
MAX_TEST_FILE_COUNT := $(words $(STD_FILES))

.DEFAULT_GOAL = all

# test all optimisation levels
.PHONY: all clean test test_clean

define TEMPLATE =
OBJS$(1) := ${SRC:.c=O$(1).o}
debugO$(1): $$(OBJS$(1)) $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -O$(1) -o $$@ $$(OBJS$(1))
mainO$(1).o: main.c $(HEADERS)
	$(CC) $(CFLAGS) $(ADFLAG) -O$(1) -o $$@ -c $$<
cpermO$(1).o: cperm.c $(HEADERS)
	$(CC) $(CFLAGS) -O$(1) -o $$@ -c $$<
endef

$(foreach optlvl, $(shell seq 0 3), $(eval $(call TEMPLATE,$(optlvl))))

clean:
	rm *.o
test_clean:
	rm *.o
	rm debug*
	rm test_results.log
all: $(addprefix debugO, $(shell seq 0 3))

test: all
	> test_results.log
	./debugO0 $(STD_FILES)
	./debugO1 $(STD_FILES)
	./debugO2 $(STD_FILES)
	./debugO3 $(STD_FILES)
	@printf "::all tests passed:: lib tested at all optimisation levels\n\n" >> test_results.log
	@printf "summary of files tested:\n" >> test_results.log
	@for file in $(STD_FILES); \
	do printf "%s\n" $${file} >> test_results.log; \
	done;
	@unset file
	@lines=$$(wc -l test_results.log | awk '{print $$1}'); \
	if [ $$lines -gt 20 ]; \
	then \
	  less test_results.log; \
	else \
	  echo ""; \
	  cat test_results.log; \
	fi; \
	unset lines;

