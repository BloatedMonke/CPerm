#ifndef _PERMUTATIONS_H_
#define _PERMUTATIONS_H_

#include <stdint.h>

/* a more fitting name for the operation */
typedef int8_t byte;

/* all of these functions will allocate memory for a new 
 * container of size (width * sizeof(object)) * (height) =
 * (k * objsize) * P(n, k) where P denotes the standard
 * permute function for each permutation listed.
 *
 * These must be freed by the caller.
 */

/* permutes the array such that both the elements
 * and the ordering are unique. i.e. only one of
 * either [1,2,3] or [3,2,1] will exist.
 *
 * The ordering takes first encountered elements
 * to be of higher priority. i.e. from the example
 * above if the call was nCKperm([1,2,3,4], 4, 3)
 * [1,2,3] would be shown rather over [3,2,1] whereas
 * for nCkperm([3,4,2,1], 4, 3) [3,2,1] would be shown.
 */
void *nCkperm(void *collection, uint8_t n, uint8_t k, uint64_t objsize);

void *cycle(void *collection, uint64_t n, uint64_t k, uint64_t objsize);

void *permutations(void *collection, uint64_t n, uint64_t k, uint64_t objsize);

/* include here so users don't have to */
#include "combinatorics.h"

#endif /* _PERMUTATIONS_H_ */