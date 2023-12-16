#include "combinatorics.h"

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
