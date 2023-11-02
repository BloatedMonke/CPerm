#include "permutations.h"
#include "combinatorics.h"

void enumerate(int *pN, int currentIter, int loopDepth, int counters[loopDepth], int loopLength[loopDepth], int arr[], int perm[]);


void *nCkperm(void *collection, uint64_t n, uint64_t k, uint64_t objsize);
{
    int counters[k], loopLength[k], iter = 0, pN = 0;
    for (int i = 1; i <= k; ++i)
    {
        counters[i - 1] = i - 1;
        loopLength[i - 1] = n - k + i;
    }

    enumerate(&pN, iter, k, counters, loopLength, arr, perm);

    return perm;
}

void swap(int *pN, int loopDepth, int counters[], int arr[], int perm[])
{
    for (int iter = 0; iter < loopDepth; ++iter)
    {
        perm[*pN * loopDepth + iter] = arr[counters[iter]];
    }
    ++*pN;
}

// automating for loops with recursion
void enumerate(int *pN, int currentIter, int loopDepth, int counters[loopDepth], int loopLength[loopDepth], int arr[], int perm[])
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
