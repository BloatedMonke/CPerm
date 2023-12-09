#ifndef _PERMUTATIONS_H_
#define _PERMUTATIONS_H_

#include <stddef.h>
#include <stdint.h>

#define   perm_group(P) ((P).group)
#define   perm_width(P) ((P).width)
#define  perm_height(P) ((P).height)
#define perm_objsize(P) ((P).objsize)

struct perm
{
    void *group;
    uint64_t height;
    uint8_t width;
    uint32_t objsize;
};

/* perm_init is to be called before any perm takes place.
 * any perm henceforth will assume that objsize until 
 * perm_init is called again with a new objsize */
void perm_init(size_t objsize);

/* all of these functions will allocate memory for a new 
 * container of size (width * objsize) * (height) =
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
 * above if the call was combinations([1,2,3,4], 4, 3)
 * [1,2,3] would be shown rather over [3,2,1] whereas
 * for combinations([3,4,2,1], 4, 3) [3,2,1] would be shown.
 */
perm combinations(void *collection, uint8_t n, uint8_t k);

/**/
perm permutations(void *collection, uint64_t n, uint64_t k);

/* free the allocated 2D perm array */
void perm_kill(perm);

/* print the perms with a  */
void print_perm(perm group, void (*pretty_print)(void *));

#endif /* _PERMUTATIONS_H_ */
