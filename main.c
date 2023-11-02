#include <stdio.h>
#include "combinatorics.h"
#include "permutations.h"

void printPerm(int *arr, int nCk, int k);

int main(void){
    int test[] = {1,2,3,4,5,6};
    int n = sizeof(test) / sizeof(*test);
    int k = 3;
    int nCk = choose(n, k);
    int perm[k * nCk];
    int *ret = nCkperm(n, k, test, perm);

    printPerm(ret, nCk, k);
}

void printPerm(int *arr, int nCk, int k){
    printf("[");
    for (int i = 0; i < nCk; ++i){
        printf("[");
        for (int j =0; j < k; ++j){
            printf("%d%s", arr[i * k + j], j < k - 1 ? ", ": "");
        }
        printf("]%s\n", i < nCk - 1 ? ",": "");
    }
    printf("]\n");
}
