CC        := gcc
SRC       := main.c cperm.c
VPATH     := ../include ../src
ADFLAG     = -DMAX_TEST_FILE_COUNT=$(MAX_TEST_FILE_COUNT)
CFLAGS     = $(INCLUDES) $(WARNINGS) $(DB_FLAGS)
HEADERS   := cperm.h combinatorics.h
INCLUDES  := ${VPATH:%=-I%}
WARNINGS  := -Wall -Wpedantic -Wextra -Werror -std=c99
DB_FLAGS  := -g3 -fsanitize=address
DATA_DIR  := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))DATA
STD_FILEN := P44char P44int P43int P42int P41int P40int \
             C44int C43int C42char C40int 
STD_FILES := ${STD_FILEN:%=$(DATA_DIR)/%.dat}
OPTLEVELS := $(shell seq 0 3)
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

$(foreach optlvl, $(OPTLEVELS), $(eval $(call TEMPLATE,$(optlvl))))

clean:
	rm *.o
test_clean:
	rm *.o
	rm debug*
	rm test_results.log
all: $(addprefix debugO, $(OPTLEVELS))

test: all
	> test_results.log
	@set -e; \
	for i in $(OPTLEVELS); do \
	  printf "(%d): " $${i}; \
	  ./debugO$${i} $(STD_FILES); \
	done;
	@printf "::all tests passed:: lib tested at all optimisation levels\n\n" >> test_results.log
	@printf "summary of files tested:\n" >> test_results.log
	@for file in $(STD_FILES); do \
	  printf "%s\n" $${file##*/} >> test_results.log; \
	done; 
	@lines=$$(wc -l test_results.log | awk '{print $$1}'); \
	if [ $$lines -gt 20 ]; then \
	  less test_results.log; \
	else \
	  echo ""; \
	  cat test_results.log; \
	fi; \

