/***
 """
    Copyright 2023 BloatedMonke
    Use of this source code is
    governed by an MIT-style license.
    Refer to the LICENSE file included.
 """
 **/

#ifndef _PERMUTATIONS_H_
#define _PERMUTATIONS_H_

#include <stddef.h>
#include <stdint.h>

struct perm;
/* TODO:: No allocs */
#if defined(STATIC_PERMS)
typedef size_t perm_buffer;
#endif


/***
 """
     All of these functions will allocate memory for a new container
    of size (width * objsize) * (height) = (k * objsize) * P(n, k)
    where P denotes the standard combinatorics function for each
    rearrangement function listed.
     These must be freed by the caller using perm_kill(P).
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
struct perm *combinations(void* collection, uint8_t n, uint8_t k, size_t size);

/*---------------------------------------------------------
 * Returns all possible rearrangemenents of the collection
 *-------------------------------------------------------*/
struct perm *permutations(void* collection, uint8_t n, uint8_t k, size_t size);

/*----------------------------------
 * free the allocated 2D perm array
 *--------------------------------*/
void perm_kill(struct perm*);

/* ======================================================
   utility functions
====================================================== */

/*---------------------------- 
 * return the 2D perm array 
 *--------------------------*/
void *perm_group(struct perm *P);

/*---------------------------- 
 * return the width of the perm
 *--------------------------*/
uint8_t perm_width(struct perm *P);

/*---------------------------------
 * return the height of the perm
 *-------------------------------*/
size_t perm_height(struct perm *P);

/*------------------------------------------------
 * return the size(s) of the permuted object(s)
 *----------------------------------------------*/
size_t perm_size(struct perm *P);

/*--------------------------------------------------------------
 * print all arrangements with a pretty printer for the objects
 *------------------------------------------------------------*/
void perm_print_all(struct perm*, void (*pretty_print)(void *));

/*--------------------------------------------------------------------
 * print the idx'th arrangement with a pretty printer for the objects
 *------------------------------------------------------------------*/
void perm_print(struct perm *group, size_t idx, void (*pretty_print)(void *));

#include <stdio.h>

/*-----------------------------------------------------------
 * similar to perm_print, but send the output to file instad
 *---------------------------------------------------------*/
void perm_fprint(FILE *file, struct perm *group, size_t idx, void (*pretty_fprint)(FILE *, void *));

#endif /* _PERMUTATIONS_H_ */
