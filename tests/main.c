#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "permutations.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <windows.h>
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

#define FMT1(fmt) "Observed(%"fmt") == Expected(%"fmt") ?"
#define TEST_FMT_STRING(observed, expected, fmt) \
        FMT1(fmt)"    %s", observed, expected, (observed) == (expected) ? GREEN"True"RESET: RED"False"RESET

void  int_assert_equals_msg(const int observed, const int expected, const char *msg);
void char_assert_equals_msg(const char observed, const char expected, const char *msg);

typedef uint8_t byte;
typedef unsigned int uint;

#define lengthof(x) (sizeof(x) / sizeof(*(x)))

/* For structs this means the actual name w/o struct */
#define MAX_TYPE_NAME_LEN 10

char perm_funcs[TEST_FILE_LEN] = {0};
const char *testers[TEST_FILE_LEN] = {NULL};
char types[TEST_FILE_LEN][MAX_TYPE_NAME_LEN+2] = {{0}};

uint Ns[TEST_FILE_LEN] = {0};
uint Ks[TEST_FILE_LEN] = {0};
uint len = 0;

FILE *files[TEST_FILE_LEN] = {NULL};

void  int_pretty_print(void *xp){ printf("%d", *(int*)xp); }
void char_pretty_print(void *xp){ printf("%c", *(char*)xp); }

void parse(void);
void run_test(void);

int main(int argc, const char *argv[])
{
    if (argc > TEST_FILE_LEN + 1) {
        printf("ERROR:: Too many arguments to %s\n", argv[0]);
        printf("        Current Max number of tester files: %d\n", TEST_FILE_LEN);
        printf("        To change this pass TEST_FILE_LEN=N to make as an argument: "
               "make TEST_FILE_LEN=N\n");
        printf("        Where N is the new max\n");
        return 1;
    }

    if (argc == 1) {
        printf("ERROR:: No truth files passed in for testing\n");
        printf("Exiting...\n");
        return 2;
    }

    if (TEST_FILE_LEN < 1) {
        printf("Max No. Tests < 1 (-_-)\n");
        return 3;
    }

    len = argc - 1;
    if (len > TEST_FILE_LEN) {
        printf("ERROR:: Passed in more than TEST_FILE_LEN=%d tests\n", TEST_FILE_LEN);
        printf("        Consider changing the TEST_FILE_LEN by passing it to make or");
        printf("decrease the number of files passed to the program\n");
        return 4;
    }

    for (uint i = 0; i < len; ++i) {
        testers[i] = argv[i+1];
        files[i] = fopen(testers[i], "r");
        if (!files[i]) {
            printf("ERROR:: Could not find %s\n", testers[i]);
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
void __attribute__((constructor)) cnstr(void)
{
#if defined(_MSC_VER) || defined(__MINGW32__)
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE) return;
    
    DWORD mode = 0;
    if (GetConsoleMode(handle, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(handle, mode);
    }
#endif
}

void parse(void)
{
    for (uint i = 0; i < len; ++i) {
        const char * const s = testers[i];
        
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
        
        int zen = s[3] == '?' ? MAX_TYPE_NAME_LEN + 1: MAX_TYPE_NAME_LEN;
        for (int j = 0; j < zen && s[j+3] != '.'; ++j)
            types[i][j] = s[j+3];
        types[i][zen] = '\0';
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

/***
 """
     The goal is to build some sort of meta template thing (possibly using generics ?)
    for dynamic type testing.
     This will have to do for now.
 """
 **/
void run_test(void)
{
    uint sizes[TEST_FILE_LEN];
    INTERNAL_PERM_T groups[TEST_FILE_LEN] = {{.group = NULL}};    

    for (uint i = 0; i < len; ++i) {
        if (strcmp(types[i], "int") == 0) {
            sizes[i] = sizeof(int);
        }
        if (strcmp(types[i], "char") == 0) {
            sizes[i] = sizeof(char);
        }
        if (strcmp(types[i], "?") == 0) {
            /* nothing we can do right now */
            sizes[i] = 0;
        }
        
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
        printf("\t\t%s\n\n", s);

        /** Run the asserts */
        
        /** Eat the lines */
        fgetc(files[i]); fgetc(files[i]);
        for (uint l = 0; l < groups[i].height; ++l) {
            j = k = 0;
            fgets(s, 3*Ns[i] - 1, files[i]);
            fgetc(files[i]);
            
            while (s[j]) {
                if (s[j] == ',' || s[j] == ' ') {
                    ++j;
                    continue;
                }
                if (strcmp(types[i], "int") == 0) {
                    num = atoi(s + j);
                    int *iarr = groups[i].group;

                    /* need to add message here */
                    int_assert_equals_msg(iarr[l * Ns[i] + k], num, "");
                }
                if (strcmp(types[i], "char") == 0) {
                    char *carr = groups[i].group;
                    
                    /* need to add message here */
                    char_assert_equals_msg(carr[l * Ns[i] + k], s[j], "");
                }
                ++j, ++k;
            }
        }
        perm_kill((struct perm *)&groups[i]);
    }

}

/*============ 
  asserts 
============*/

/* As of right now perms won't die on early exit. Add a fix then bring back early 
 * exit on fail
 */

void int_assert_equals_msg(const int observed, const int expected, const char *msg)
{
    printf(TEST_FMT_STRING(observed, expected, "d"));
    puts("");

    if (observed != expected){
        printf("%s\n", msg);
#if 0
        printf("exiting...\n");
        exit(0xff);
#endif
    }
}

void char_assert_equals_msg(const char observed, const char expected, const char *msg){
    printf(TEST_FMT_STRING(observed, expected, "c"));
    puts("");

    if (observed != expected){
        printf("%s\n", msg);
#if 0
        printf("exiting...\n");
        exit(0xff);
#endif
    }
}
