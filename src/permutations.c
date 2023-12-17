#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "combinatorics.h"
#include "permutations.h"

#define   get_perm_group(P) ((P)->group)
#define   get_perm_width(P) ((P)->width)
#define  get_perm_height(P) ((P)->height)
#define get_perm_objsize(P) ((P)->objsize)

typedef uint8_t byte;
typedef unsigned int uint;
typedef struct perm perm;

void perm_kill(perm *A)
{
    free(get_perm_group(A));
}

static inline void base_perm_swap(uint A[], int i, int j)
{
    A[i] ^= A[j];
    A[j] ^= A[i];
    A[i] ^= A[j];
}

perm permutations(void *collection, uint8_t n, uint8_t k, size_t size)
{
    uint64_t height = nPr(n, k);

    uint seed[n], cycles[k], rowN = 0;
    for (uint i = 0; i < n; ++i) seed[i] = i;
    for (uint i = 0; i < k; ++i) cycles[i] = n - i;

    byte *group = malloc(height * k * size);
    byte *pool = (byte *)collection;
    for (uint i = 0; i < k; ++i)
        memcpy(group + (rowN * k + i) * size, pool + (seed[i]) * size, size);
    ++rowN;
    
    bool no_break = false;
    while (true) {
        if (no_break) break;
        
        for (int i = k-1; i >= 0; --i) {
            cycles[i] -= 1;
            
            if (cycles[i] == 0) {
                uint tmp = seed[i];
                
                for (uint j = i + 1; j < k; ++j)
                    seed[j-1] = seed[j];
        
                seed[k-1] = tmp;
                cycles[i] = n - i;
            }
            
            else {
                base_perm_swap(seed, i, n - cycles[i]);
                for (uint j = 0; j < k; ++j)
                    memcpy(group + (rowN * k + j) * size, pool + (seed[j]) * size, size);
                
                ++rowN;
                no_break = false;
                break;
            }
            no_break = true;
        }
    }
    return (perm){.group = group, .width = k, .height = height, .objsize = size};
}

/*-------------------------------------
 * automating for loops with recursion
 *-----------------------------------*/
static inline void
rearrange(int * __restrict rowN, int ijk, int k, int counters[k], int ijk_ends[k],
          byte * __restrict arr, byte * __restrict group, size_t size)
{
    if (ijk == k) {
        for (int iter = 0; iter < k; ++iter)
            memcpy(group + (*rowN * k + iter) * size, arr + counters[iter] * size, size);

        ++*rowN;
        return;
    }

    /** set counters to their new state and continue the loop */
    for (counters[ijk] = (counters[ijk - 1 * (ijk > 0)] + 1) * (ijk > 0);
         counters[ijk] < ijk_ends[ijk];
         ++counters[ijk])

        rearrange(rowN, ijk + 1, k, counters, ijk_ends, arr, group, size);

    return;
}

perm combinations(void *collection, uint8_t n, uint8_t k, size_t size)
{
    uint64_t height = nCk(n, k);
    
    byte *group = malloc(height * k * size);

    if (size < 1 || !group) {
        height = k = 0;
        free(group);
        group = NULL;
    }

    /** setup loop variables */
    int counters[k], ijk_ends[k], rowN = 0;
    for (int i = 0; i < k; ++i) {
        counters[i] = i;
        ijk_ends[i] = n - k + i + 1;
    }

    rearrange(&rowN, 0, k, counters, ijk_ends, collection, group, size);

    return (perm){.group = group, .width = k, .height = height, .objsize = size};
}

/* =====================================================
 * utility funcs
 ==================================================== */

#include <stdio.h>

void print_perm(perm *group, void (*pretty_print)(void *))
{
    printf("[\n");
    for (uint i = 0; i < get_perm_height(group); ++i){
        printf("[");
        for (uint j = 0; j < get_perm_width(group); ++j){
            pretty_print( (void *)&((byte *)get_perm_group(group))[ (i * get_perm_width(group) + j) * get_perm_objsize(group)] );
            printf("%s", j < get_perm_width(group) - 1u ? ", ": "");
        }
        printf("]%s\n", i < get_perm_height(group) - 1 ? ",": "");
    }
    printf("]\n");
}
