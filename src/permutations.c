#include <stdlib.h>
#include <string.h>
#include "combinatorics.h"
#include "permutations.h"

typedef uint8_t byte;
typedef unsigned int uint;
typedef struct perm perm;

extern size_t PERM_OBJ_SIZE;

void perm_init(size_t objsize)
{
    PERM_OBJ_SIZE = objsize;
}

void perm_kill(perm A)
{
    free(perm_group(A));
}

/* automating for loops with recursion */
void enumerate(int * restrict rowN, int ijk, int k, int counters[k], int ijk_ends[k], byte * restrict arr, byte * restrict group)
{
    if (ijk == k) {
        for (int iter = 0; iter < k; ++iter)
            memcpy(group + (*rowN * k + iter) * PERM_OBJ_SIZE, collection + counters[iter] * PERM_OBJ_SIZE, PERM_OBJ_SIZE);

        ++*rowN;
        return;
    }

    /** set counters to their new state and continue the loop */
    for (counters[ijk] = (counters[ijk - 1 * (ijk > 0)] + 1) * (ijk > 0);
         counters[ijk] < ijk_ends[ijk];
         ++counters[ijk])

        enumerate(rowN, ijk + 1, k, counters, ijk_ends, arr, group);

    return;
}

perm combinations(void *collection, uint8_t n, uint8_t k)
{
    uint64_t height = choose(n, k);
    
    byte *group = malloc(height * k * PERM_OBJ_SIZE);

    /** setup loop variables */
    int counters[k], ijk_ends[k], rowN = 0;
    for (int i = 0; i < k; ++i)
    {
        counters[i] = i;
        ijk_ends[i] = n - k + i + 1;
    }

    enumerate(&rowN, 0, k, counters, ijk_ends, collection, group);

    return (perm){.group = group, .width = k, .height = height, .objsize = PERM_OBJ_SIZE};
}

void base_perm_swap(uint A[], int i, int j)
{
    A[i] ^= A[j];
    A[j] ^= A[i];
    A[i] ^= A[j];
}

/* TODO */
void cycle(byte * restrict collection, byte * restrict group, uint64_t n, uint64_t k, uint base_perm[k])
{
    /* PHONY: */
    n += k;
    collection ?
    ++base_perm[0]: ++base_perm[1];
    group ? ++base_perm[1]: ++base_perm[2];
    /* END PHONY */
}

perm permutations(void *collection, uint64_t n, uint64_t k)
{
    uint64_t height = nPr(n, k);

    uint seed[k];
    for (uint i = 0; i < k; ++i) seed[i] = i;

    byte *group = malloc(height * k * PERM_OBJ_SIZE);

    for (uint j = 0; j < k; ++j) {
        cycle(collection, group, n, k, seed);
        base_perm_swap(seed, 1, j);
    }

    return (perm){.group = group, .width = k, .height = height, .objsize = PERM_OBJ_SIZE};
}

/* =====================================================
 * utility funcs
 ==================================================== */

#include <stdio.h>

void print_perm(perm group, void (*pretty_print)(void *))
{
    printf("[\n");
    for (uint i = 0; i < group.height; ++i){
        printf("[");
        for (uint j = 0; j < group.width; ++j){
            pretty_print( (void *)&((byte *)group.group)[ (i * group.width + j) * group.objsize] );
            printf("%s", j < group.width - 1u ? ", ": "");
        }
        printf("]%s\n", i < group.height - 1 ? ",": "");
    }
    printf("]\n");
}
