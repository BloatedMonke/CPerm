#include "combinatorics.h"

int choose(int n, int k)
{
    return flooredFact(n, k) / fact(k);
}

int flooredFact(int n, int k)
{
    if (k == 0) return 1;
    int p = 1;
    for (int i = n - k + 1; i < n; ++i)
    {
        p *= i;
    }
    return n * p;
}

int fact(int n)
{
    if (n == 0) return 1;
    int p = 1;
    for (int i = 2; i < n; ++i)
    {
        p *= i;
    }
    return n * p;
}
