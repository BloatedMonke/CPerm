#include "combinatorics.h"

uint64_t choose(uint64_t n, uint64_t k)
{
    return flooredFact(n, k) / fact(k);
}

// TODO
int64_t choose_e(int64_t n, uint64_t k)
{
    return -1;
}

uint64_t flooredFact(uint64_t n, uint64_t k)
{
    if (k == 0) return 1;
    uint64_t p = 1;
    for (uint8_t i = n - k + 1; i < n; ++i)
    {
        p *= i;
    }
    return n * p;
}

uint64_t fact(uint64_t n)
{
    if (n == 0) return 1;
    uint64_t p = 1;
    for (uint8_t i = 2; i < n; ++i)
    {
        p *= i;
    }
    return n * p;
}
