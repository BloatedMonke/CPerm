#include "combinatorics.h"

uint64_t choose(uint64_t n, uint64_t k)
{
    return flooredFact(n, k) / fact(k);
}

uint64_t flooredFact(uint64_t n, uint64_t k)
{
    if (k == 0) return 1;
    int p = 1;
    for (int i = n - k + 1; i < n; ++i)
    {
        p *= i;
    }
    return n * p;
}

uint64_t fact(uint64_t n)
{
    if (n == 0) return 1;
    int p = 1;
    for (int i = 2; i < n; ++i)
    {
        p *= i;
    }
    return n * p;
}
