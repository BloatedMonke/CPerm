#include <stdio.h>
#include <stdlib.h>
#include "combinatorics.h"
#include "permutations.h"

void printPerm(void *arr, int nCk, int k, int objsize, void (*prettyPrint)(void *));

int main(void){
    int test[] = {1,2,3,4,5,6};
    int n = sizeof(test) / sizeof(*test);
    int k = 3;
    int *ret = nCkperm(test, n, k, sizeof(*test));
    void lambda(void *x){printf("%d", *(int *)x);}
    printPerm(ret, choose(n, k), k, sizeof(*test), lambda);

    printf("---------------------------\n");

    char s[] = "ABCDEFGHIJKLMNOPQ";
    int n2 = sizeof(s) / sizeof(*s) - 1;
    int k2 = 3;
    char *sT = nCkperm(s, n2, k2, sizeof(*s));
    void lambda2(void *x){printf("%c", *(char *)x);}
    printPerm(sT, choose(n2, k2), k2, sizeof(*sT), lambda2);

    free(ret);
    free(sT);
    return 0;
}

void printPerm(void *arr, int nCk, int k, int objsize, void (*prettyPrint)(void *)){
    printf("[\n");
    for (int i = 0; i < nCk; ++i){
        printf("[");
        for (int j =0; j < k; ++j){
            prettyPrint( (void *)&((byte *)arr)[(i * k + j) * objsize] );
            printf("%s", j < k - 1 ? ", ": "");
        }
        printf("]%s\n", i < nCk - 1 ? ",": "");
    }
    printf("]\n");
}
