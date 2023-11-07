#include <stdio.h>
#include <stdlib.h>
#include "permutations.h"

#define countof(x) (sizeof(x) / sizeof(*(x)))
#define lengthof(s) (countof(s) - 1)

int main(void){
    int test[] = {1,2,3,4,5,6};
    int k = 3;
    permptr ret = nCkperm(test, countof(test), k, sizeof(*test));
    
    void lambda(void *x){printf("%d", *(int *)x);}
    printPerm(ret, lambda);

    printf("---------------------------\n");


    char s[] = "ABCDEFGHIJKLMNOPQRST";
    int k2 = 10;
    permptr sT = nCkperm(s, lengthof(s), k2, sizeof(*s));
    
    void lambda2(void *x){printf("%c", *(char *)x);}
    printPerm(sT, lambda2);

    kill_perm(ret);
    kill_perm(sT);
    return 0;
}
