#include <stdlib.h>
#include <string.h>
#include "permutations.h"
#include "combinatorics.h"

struct perm
{
    void *collection;
    uint64_t height;
    uint8_t width;
    uint32_t objsize;
};
typedef struct perm
perm;

permptr birth_perm(uint64_t height, uint8_t width, uint32_t objsize)
{
    permptr new = malloc(sizeof(perm));
    new->collection = (void *)0;
    new->height = height;
    new->width = width;
    new->objsize = objsize;
    return new;
}

void kill_perm(permptr p)
{
    free(p->collection);
    free(p);
}

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

permptr nCkperm(void *collection, uint8_t n, uint8_t k, uint64_t objsize)
{
    uint64_t height = choose(n, k);
    permptr group = birth_perm(height, k, objsize);
    
    // byte * enables manipulation of objs
    byte *perm = malloc(height * k * objsize);

    // setup loop variables for recursion
    int counters[k], ijk_ends[k], rowN = 0;
    for (int i = 0; i < k; ++i)
    {
        counters[i] = i;
        ijk_ends[i] = n - k + i + 1;
    }

    enumerate(&rowN, 0, k, counters, ijk_ends, collection, perm, objsize);

    group->collection = perm;

    return group;
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
permptr permutations(void *collection, uint64_t n, uint64_t k, uint64_t objsize)
{
    // PHONY:
    n += k * objsize;
    return collection;
    // END PHONY
}

/* =====================================================
 * utility funcs
 ==================================================== */

#include <stdio.h>

void printPerm(permptr group, void (*prettyPrint)(void *)){
    printf("[\n");
    for (int i = 0; i < group->height; ++i){
        printf("[");
        for (int j =0; j < group->width; ++j){
            prettyPrint( (void *)&((byte *)group->collection)[ (i * group->width + j) * group->objsize] );
            printf("%s", j < group->width - 1 ? ", ": "");
        }
        printf("]%s\n", i < group->height - 1 ? ",": "");
    }
    printf("]\n");
}
