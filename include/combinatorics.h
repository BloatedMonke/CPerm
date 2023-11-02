#ifndef _COMB_H_
#define _COMB_H_

#include <stdint.h>

/**/
uint64_t choose(uint64_t n, uint64_t k);

/**/
int64_t choose_e(int64_t n, uint64_t k);

/**/
uint64_t flooredFact(uint64_t n, uint64_t k);

/**/
uint64_t fact(uint64_t n);

#endif /* _COMB_H_ */