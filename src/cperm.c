/***
 """
    Copyright 2023 BloatedMonke
    Use of this source code is
    governed by an MIT-style license.
    Refer to the LICENSE file included.
 """
 **/

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "combinatorics.h"
#include "cperm.h"

#define   get_pg(P) ((P)->group)
#define   get_pw(P) ((P)->width)
#define   get_ph(P) ((P)->height)
#define  get_pos(P) ((P)->objsize)

typedef unsigned char byte;
typedef unsigned int uint;
typedef struct perm perm;
struct internal_perm {
    void*     group;
    uint64_t height;
    uint8_t   width;
    size_t  objsize;
};
typedef struct internal_perm
INTERNAL_PERM_T;

void perm_kill(perm *A)
{
    free(get_pg(A));
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

    INTERNAL_PERM_T circle;
    byte *group;
    byte *pool;

    bool no_break = false;
    group = malloc(height * k * size);
    pool = (byte *)collection;

    if (0 == size || !group || k > n || 0 == k) {
        free(group);
        circle.group = NULL;
        circle.width = 0;
        circle.height = 1;
        circle.objsize = 0;
        n = k = 0;
        no_break = true;
    }
    

    uint seed[n], cycles[k], rowN;
    for (uint i = 0; i < n; ++i) seed[i] = i;
    for (uint i = 0; i < k; ++i) cycles[i] = n - i;
    for (uint i = 0; i < k; ++i) memcpy(group + i * size, pool + (seed[i]) * size, size);
    
    rowN = 1;
    while (true) {
        if (no_break) break;
        
        for (int i = k-1; i >= 0; --i) {
            cycles[i] -= 1;
            
            if (cycles[i] != 0) {
                base_perm_swap(seed, i, n - cycles[i]);
                for (uint j = 0; j < k; ++j)
                    memcpy(group + (rowN * k + j) * size, pool + (seed[j]) * size, size);
                ++rowN;
                no_break = false;
                break;
            }            
            for (uint j = i + 1; j < n; ++j)
                base_perm_swap(seed, j-1, j);
    
            cycles[i] = n - i;
            no_break = true;            
        }
    }
    if (k <= n && 0 != k) {
        circle.group = group;
        circle.width = k;
        circle.height = height;
        circle.objsize = size;
    }
    return *(struct perm *)&circle;
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

    if (0 == size || !group || k > n || 0 == k) {
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

/* ========================
 * Combinatorics
 ========================*/

uint64_t nCk(uint64_t n, uint64_t k)
{
    return nPr(n, k) / fact(k);
}

uint64_t nPr(uint64_t n, uint64_t r)
{
    if (r <= 0) return 1;
    uint64_t p = 1;
    for (uint8_t i = n - r + 1; i <= n; ++i) {
        p *= i;
    }
    return p;    
}

uint64_t fact(uint64_t n)
{
    if (n == 0) return 1;
    uint64_t p = 1;
    for (uint8_t i = 2; i <= n; ++i) {
        p *= i;
    }
    return p;
}


/* ========================
 * Utility
 ========================*/

#include <stdio.h>

void perm_print_all(perm *group, void (*pretty_print)(void *))
{
    printf("[\n");
    for (uint i = 0; i < get_ph(group); ++i) {
        printf("[");
        for (uint j = 0; j < get_pw(group); ++j) {
            pretty_print( (void *)&((byte *)get_pg(group))[ (i * get_pw(group) + j) * get_pos(group)] );
            printf("%s", j < get_pw(group) - 1u ? ", ": "");
        }
        printf("]%s\n", i < get_ph(group) - 1 ? ",": "");
    }
    printf("]\n");
}

void perm_print(perm *group, size_t idx, void (*pretty_print)(void *))
{
    if (idx > get_ph(group)) {fprintf(stderr, "ERROR:: index out of rangr\n" ); exit(0xf9);}
    printf("[");
    for (uint j = 0; j < get_pw(group); ++j) {
        pretty_print( (void *)&((byte *)get_pg(group))[ (idx * get_pw(group) + j) * get_pos(group)] );
        printf("%s", j < get_pw(group) - 1u ? ", ": "");
    }
    printf("]\n");
}

void perm_fprint(FILE *file, perm *group, size_t idx, void (*pretty_fprint)(FILE *, void *))
{
    if (idx > get_ph(group)) {fprintf(stderr, "ERROR:: index out of rangr\n" ); exit(0xf9);}
    fprintf(file, "[");
    for (uint j = 0; j < get_pw(group); ++j) {
        pretty_fprint(file, (void *)&((byte *)get_pg(group))[ (idx * get_pw(group) + j) * get_pos(group)] );
        fprintf(file, "%s", j < get_pw(group) - 1u ? ", ": "");
    }
    fprintf(file, "]\n");
}
