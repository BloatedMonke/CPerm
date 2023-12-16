#ifndef _PERMUTATIONS_H_
#define _PERMUTATIONS_H_

#include <stddef.h>
#include <stdint.h>

#define   perm_group(P) ((P).group)
#define   perm_width(P) ((P).width)
#define  perm_height(P) ((P).height)
#define perm_objsize(P) ((P).objsize)

struct perm {
    void*           group;
    const uint64_t height;
    const uint8_t   width;
    const size_t  objsize;
};

/***
 """
     All of these functions will allocate memory for a new container
    of size (width * objsize) * (height) = (k * objsize) * P(n, k)
    where P denotes the standard combinatorics function for each
    rearrangement function listed.
     These must be freed by the caller using perm_kill(&P).
 """
 **/

/*----------------------------------------------------------------------
 *  Permutes the array such that only the elements of each rearrangement
 * are unique. i.e. , if the call was combinations([1,2,3,4], 4, 3),
 * only one of either [1,2,3] or [3,2,1] will exist.
 *  The ordering takes first encountered elements to be of higher
 * priority. i.e. from the example above [1,2,3] would be preferred
 * over [3,2,1]. Whereas for combinations([3,4,2,1], 4, 3) [3,2,1]
 * would be shown.
 *--------------------------------------------------------------------*/
struct perm combinations(void* collection, uint8_t n, uint8_t k, size_t size);

/*---------------------------------------------------------
 * Returns all possible rearrangemenents of the collection
 *-------------------------------------------------------*/
struct perm permutations(void* collection, uint8_t n, uint8_t k, size_t size);

/*----------------------------------
 * free the allocated 2D perm array
 *--------------------------------*/
void perm_kill(struct perm*);

/* ======================================================
   utility functions
====================================================== */

/*-------------------------------------------------------
 * print the perms with a pretty printer for the objects
 *-----------------------------------------------------*/
void print_perm(struct perm*, void (*pretty_print)(void *));

#endif /* _PERMUTATIONS_H_ */
