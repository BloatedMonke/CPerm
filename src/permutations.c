#include <stdlib.h>
#include "permutations.h"
#include "combinatorics.h"

void enumerate(int *pN, int currentIter, int loopDepth, int counters[loopDepth], int loopLength[loopDepth], byte *arr, byte *perm);


void *nCkperm(byte *collection, uint64_t n, uint64_t k, uint64_t objsize);
{
    // byte * enables manipulation of objs
    byte *perm = malloc(k * objsize * choose(n, k));

    int counters[k], loopLength[k], pN = 0;
    for (int i = 0; i < k; ++i)
    {
        counters[i] = i;
        loopLength[i] = n - k + i + 1;
    }

    enumerate(&pN, 0, k, counters, loopLength, arr, perm);

    return perm;
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
void enumerate(int *pN, int currentIter, int loopDepth, int counters[loopDepth], int loopLength[loopDepth], byte *arr, byte *perm)
{
    if (currentIter == loopDepth)
    {
        swap(pN, loopDepth, counters, arr, perm);
        return;
    }

    // set counters to their new state and continue the loop
    for (counters[currentIter] = (counters[currentIter - 1] + 1) * (currentIter > 0);
         counters[currentIter] < loopLength[currentIter];
         ++counters[currentIter])

        enumerate(pN, currentIter + 1, loopDepth, counters, loopLength, arr, perm);

    return;
}
