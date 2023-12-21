/***
 """
    Copyright 2023 BloatedMonke
    Use of this source code is
    governed by an MIT-style license.
    Refer to the LICENSE file included.
 """
 **/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cperm.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
HANDLE WINAPI   GetStdHandle(_In_ DWORD  nStdHandle);
BOOL   WINAPI GetConsoleMode(_In_ HANDLE hConsoleHandle, _Out_ LPDWORD lpMode);
BOOL   WINAPI SetConsoleMode(_In_ HANDLE hConsoleHandle, _In_ DWORD    dwMode);
#endif

typedef unsigned char byte;
typedef unsigned int  uint;

#define RED              "\x1b[1;31m"
#define RESET            "\x1b[0m"
#define PURPLE           "\x1b[1;95m"
#define UNDERLINE        "\x1b[4;37m"
#define RED_UNDERLINE    "\x1b[4;31m"

#define FMT1(fmt)        "Observed(%"fmt") == Expected(%"fmt") ?"
#define TEST_FMT_STRING(observed, expected, fmt) \
        FMT1(fmt)"    %s", observed, expected, (observed) == (expected) ? "": RED"False"RESET

/* Does not include structs */
#define MAX_TYPE_NAME_LEN 10
#define STRUCT_IDENTIFIER "-"

                /* source of truths */
const char*             sots[MAX_TEST_FILE_COUNT] = {NULL};
char                 perm_funcs[MAX_TEST_FILE_COUNT] = {0};
char types[MAX_TEST_FILE_COUNT][MAX_TYPE_NAME_LEN+2] = {{0}};

uint      Ns[MAX_TEST_FILE_COUNT] = {0};
uint      Ks[MAX_TEST_FILE_COUNT] = {0};
size_t sizes[MAX_TEST_FILE_COUNT] = {0};
uint len = 0;

FILE *files[MAX_TEST_FILE_COUNT] = {NULL};

bool  int_assert_equals_msg(const int  observed, const int  expected, const char *msg);
bool char_assert_equals_msg(const char observed, const char expected, const char *msg);

const char *ordstem(uint x);
static inline void pexit(uint l, uint k, const char* const s, struct perm* groups, uint i);
void  int_pretty_fprint(FILE *file, void *xp){ fprintf(file, "%d", *(int *)xp); }
void char_pretty_fprint(FILE *file, void *xp){ fprintf(file, "%c", *(char*)xp); }

void parse(void);
void run_test(void);

int main(int argc, const char *argv[])
{
    if (argc > MAX_TEST_FILE_COUNT + 1) {
        fprintf(stderr, RED"ERROR"RESET":: Too many arguments to %s\n", argv[0]);
        fprintf(stderr, "        Current Max number of tester files: %d\n", MAX_TEST_FILE_COUNT);
        fprintf(stderr, "        You passed in %d\n", argc - 1);
        fprintf(stderr, "        To change this pass MAX_TEST_FILE_COUNT=N to make as an argument:\n"
                        "        \t\tmake test MAX_TEST_FILE_COUNT=N\n");
        fprintf(stderr, "        Where N is the new max\n");
        return 1;
    }

    if (argc == 1) {
        fprintf(stderr, RED"ERROR"RESET":: No truth files passed in for testing\n");
        fprintf(stderr, "Exiting...\n");
        return 2;
    }

    if (MAX_TEST_FILE_COUNT < 1) {
        fprintf(stderr, RED"ERROR"RESET":: Max No. Tests < 1 (-_-)\n");
        return 3;
    }

    len = argc - 1;
    for (uint i = 0; i < len; ++i) {
        sots[i] = argv[i+1];
        files[i] = fopen(sots[i], "r");
        if (!files[i]) {
            fprintf(stderr, RED"ERROR"RESET":: Could not find %s\n", sots[i]);
            return 5;
        }
    }

    parse();
    run_test();

    for (uint i = 0; i < len; ++i)
        fclose(files[i]);

    return 0;
}

/* set colours */
#if defined(_MSC_VER) || defined(__MINGW32__)
void __attribute__((constructor))
cnstr(void)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE) return;
    
    DWORD mode = 0;
    if (GetConsoleMode(handle, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(handle, mode);
    }
}
#endif

void parse(void)
{
    for (uint i = 0; i < len; ++i) {
        const char * const s = sots[i];
        
        /* Grab the func */
        perm_funcs[i] = s[0];
        
        /* Grab n */
        Ns[i] = s[1] - '0';
        
        /* Grab k */
        Ks[i] = s[2] - '0';

        /* Now grab the type */
        if (!strchr(s+3, '.')) {
            printf("add a file extension (.txt will do) to %s i.e arg %d\n", s, i+1);
            exit(6);
        }
        
        for (int j = 0; j < MAX_TYPE_NAME_LEN && s[j+3] != '.'; ++j)
            types[i][j] = s[j+3];
        types[i][MAX_TYPE_NAME_LEN] = '\0';

        if (strcmp(types[i], "int") == 0) {
            sizes[i] = sizeof(int);
        }
        if (strcmp(types[i], "char") == 0) {
            sizes[i] = sizeof(char);
        }
        if (strcmp(types[i], STRUCT_IDENTIFIER) == 0) {
            sizes[i] = atoi(s+4);
            freopen(sots[i], "rb", files[i]);
        }
    }
}

struct internal_perm {
    void*     group;
    uint64_t height;
    uint8_t   width;
    size_t  objsize;
};
typedef struct internal_perm
INTERNAL_PERM_T;

struct anon {
    char pad;
    byte data[];
} __attribute__((aligned(__alignof__(long double))));
typedef struct anon
INTERNAL_ANON_T;

void run_test(void)
{
    INTERNAL_PERM_T groups[MAX_TEST_FILE_COUNT] = {{.group = NULL}};    

    for (uint i = 0; i < len; ++i) {
        /** Grab the array */
        char s[3*Ns[i] - 1];
        int j = 0, k = 0, num;
        byte arr[Ns[i] * sizes[i]];
        
        fgets(s, 3*Ns[i] - 1, files[i]);
        while (s[j]) {
            if (s[j] == ',' || s[j] == ' ') {
                ++j;
                continue;
            }
            if (strcmp(types[i], "int") == 0) {
                num = atoi(s + j);
                memcpy(arr + k * sizes[i], &num, sizes[i]);
            }
            if (strcmp(types[i], "char") == 0) {
                arr[k * sizes[i]] = (byte)s[j];
            }
            ++j, ++k;
        }

        if (perm_funcs[i] == 'P'){
            struct perm touch = permutations(arr, Ns[i], Ks[i], sizes[i]);
            groups[i] = *(INTERNAL_PERM_T *)&touch;
        }
        if (perm_funcs[i] == 'C'){
            struct perm touch = combinations(arr, Ns[i], Ks[i], sizes[i]);
            groups[i] = *(INTERNAL_PERM_T *)&touch;
        }

        /* format */
        if (i > 0) puts("");
        printf("\t\t%s\tn = %d  k = %d\n\n", s, Ns[i], Ks[i]);

        /** Run the asserts */
        
        /** Eat the lines */
        fgetc(files[i]); fgetc(files[i]);
        for (uint l = 0; l < groups[i].height; ++l) {
            j = k = 0;
            fgets(s, 3*Ks[i] - 1, files[i]);
            fgetc(files[i]);
            
            while (s[j]) {
                if (s[j] == ',' || s[j] == ' ') {
                    ++j;
                    continue;
                }
                if (strcmp(types[i], "int") == 0) {
                    num = atoi(s + j);
                    int *iarr = groups[i].group;
                    if (iarr == NULL && Ks[i] != 0)
                        pexit(l, k, s, (struct perm *)groups, i);
                    if (iarr != NULL)
                    /* add msg when the asserts take fmts */
                    if (!int_assert_equals_msg(iarr[l * Ks[i] + k], num, "")) {
                        pexit(l, k, s, (struct perm *)groups, i);
                    }
                }
                if (strcmp(types[i], "char") == 0) {
                    char *carr = groups[i].group;
                    if (carr == NULL && Ks[i] != 0)
                        pexit(l, k, s, (struct perm *)groups, i);
                    if (carr != NULL)
                    /* add msg when the asserts take fmts */
                    if (!char_assert_equals_msg(carr[l * Ks[i] + k], s[j], "")) {
                        pexit(l, k, s, (struct perm *)groups, i);
                    }
                }
                ++j, ++k;
            }
        }
        printf("::all equal::\n");
        perm_kill((struct perm *)&groups[i]);
    }
}

static inline void
pexit(uint l, uint k, const char* const s, struct perm* groups, uint i)
{
    fprintf(stderr, RED"::FAILURE::%s"RESET PURPLE" %d%s Line, %d%s Row, %d%s Column:\nExpected [%s] but observed "RESET, sots[i], l+3, ordstem(l+3), l+1, ordstem(l+1), k+1, ordstem(k+1), s);
    
    /* TODO:: use ncurses so that only the k'th symbol gets underlined*/
    fprintf(stderr, RED_UNDERLINE);
    perm_fprint(stderr, (groups+i), l, char_pretty_fprint);
    fprintf(stderr, RESET);
    exit(0xff);
}

/*============ 
  Asserts 
============*/

/* TODO::
 *     - let these take fmt strings
 */

bool int_assert_equals_msg(const int observed, const int expected, const char *msg)
{
    if (observed != expected){
        fprintf(stderr, TEST_FMT_STRING(observed, expected, "d"));
        printf("\n");
        printf("%s\n", msg);
        return false;
    }
    return true;
}

bool char_assert_equals_msg(const char observed, const char expected, const char *msg)
{
    if (observed != expected){
        fprintf(stderr, TEST_FMT_STRING(observed, expected, "c"));
        printf("\n");
        printf("%s\n", msg);
        return false;
    }
    return true;
}

/*============
  Utility
 ===========*/

const char *ordstem(uint x)
{
    switch (x%20) {
    case 0:  return "th";
    case 1:  return "st";
    case 2:  return "nd";
    case 3:  return "rd";
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19: return "th";
    default: break;
    }
    return ""; /* make the compiler happy */
}
