#include <stdlib.h>
#include <string.h>
#include "permutations.h"
#include "combinatorics.h"

void enumerate(int *pN, int currentIter, int loopDepth, int counters[loopDepth], int loopLength[loopDepth], byte *arr, byte *perm, uint64_t objsize);


void *nCkperm(void *collection, uint8_t n, uint8_t k, uint64_t objsize)
{
    // byte * enables manipulation of objs
    byte *perm = malloc(k * objsize * choose(n, k));

    // setup loop variables for recursion
    int counters[k], loopLength[k], pN = 0;
    for (int i = 0; i < k; ++i)
    {
        counters[i] = i;
        loopLength[i] = n - k + i + 1;
    }

    enumerate(&pN, 0, k, counters, loopLength, collection, perm, objsize);

    return perm;
}

// TODO
void *cycle(void *collection, uint64_t n, uint64_t k, uint64_t objsize)
{

}

// TODO
void *permutations(void *collection, uint64_t n, uint64_t k, uint64_t objsize)
{

}

void swap(int *pN, int loopDepth, int counters[], byte *collection, byte *perm, uint64_t objsize)
{
    for (int iter = 0; iter < loopDepth; ++iter)
    {
        memcpy(perm + (*pN * loopDepth + iter) * objsize, collection + counters[iter] * objsize, objsize);
    }
    ++*pN;
}

// automating for loops with recursion
void enumerate(int *pN, int currentIter, int loopDepth, int counters[loopDepth], int loopLength[loopDepth], byte *arr, byte *perm, uint64_t objsize)
{
    if (currentIter == loopDepth)
    {
        swap(pN, loopDepth, counters, arr, perm, objsize);
        return;
    }

    // set counters to their new state and continue the loop
    for (counters[currentIter] = (counters[currentIter - 1 * (currentIter > 0)] + 1) * (currentIter > 0);
         counters[currentIter] < loopLength[currentIter];
         ++counters[currentIter])

        enumerate(pN, currentIter + 1, loopDepth, counters, loopLength, arr, perm, objsize);

    return;
}
