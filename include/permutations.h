#ifndef _PERMUTATIONS_H_
#define _PERMUTATIONS_H_

#include "combinatorics.h"

int *nCkperm(int n, int k, int arr[n], int perm[k * choose(n, k)]);
int *cyclicperm(int n, int k, int arr[n], int perm[]);
int *permutations(int n, int k, int arr[n], int perm[]);

#endif /* _PERMUTATIONS_H_ */
