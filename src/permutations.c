#include <stdlib.h>
#include <string.h>
#include "permutations.h"
#include "combinatorics.h"

void swap(int *rowN, int k, int counters[], byte *collection, byte *perm, uint64_t objsize)
{
    for (int iter = 0; iter < k; ++iter)
    {
        memcpy(perm + (*rowN * k + iter) * objsize, collection + counters[iter] * objsize, objsize);
    }
    ++*rowN;
}

// automating for loops with recursion
void enumerate(int *rowN, int ijk, int k, int counters[k], int ijk_ends[k], byte *arr, byte *perm, uint64_t objsize)
{
    if (ijk == k)
    {
        swap(rowN, k, counters, arr, perm, objsize);
        return;
    }

    // set counters to their new state and continue the loop
    for (counters[ijk] = (counters[ijk - 1 * (ijk > 0)] + 1) * (ijk > 0);
         counters[ijk] < ijk_ends[ijk];
         ++counters[ijk])

        enumerate(rowN, ijk + 1, k, counters, ijk_ends, arr, perm, objsize);

    return;
}

perm nCkperm(void *collection, uint8_t n, uint8_t k, uint64_t objsize)
{
    uint64_t height = choose(n, k);
    
    // byte * enables manipulation of objs
    byte *group = malloc(height * k * objsize);

    // setup loop variables for recursion
    int counters[k], ijk_ends[k], rowN = 0;
    for (int i = 0; i < k; ++i)
    {
        counters[i] = i;
        ijk_ends[i] = n - k + i + 1;
    }

    enumerate(&rowN, 0, k, counters, ijk_ends, collection, group, objsize);

    return (perm){.collection = group, .width = k, .height = height, .objsize = objsize};
}

// TODO
void *cycle(void *collection, uint64_t n, uint64_t k, uint64_t objsize)
{
    // PHONY:
    n += k * objsize;
    return collection;
    // END PHONY
}

// TODO
perm permutations(void *collection, uint64_t n, uint64_t k, uint64_t objsize)
{
    // PHONY:
    n += k * objsize;
    return (perm){.collection = collection};
    // END PHONY
}

/* =====================================================
 * utility funcs
 ==================================================== */

#include <stdio.h>

void printPerm(perm group, void (*prettyPrint)(void *)){
    printf("[\n");
    for (uint i = 0; i < group.height; ++i){
        printf("[");
        for (uint j = 0; j < group.width; ++j){
            prettyPrint( (void *)&((byte *)group.collection)[ (i * group.width + j) * group.objsize] );
            printf("%s", j < group.width - 1u ? ", ": "");
        }
        printf("]%s\n", i < group.height - 1 ? ",": "");
    }
    printf("]\n");
}
