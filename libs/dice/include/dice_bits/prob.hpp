#pragma once

#include <cassert>

namespace dice::prob
{
// Binomial coefficient
// n choose k
// n! / (k! * (n - k)!)
unsigned int binom(const unsigned int n, const unsigned int k)
{
    assert(n >= k);
    assert(k > 0);

    unsigned int r = 1;
    for (unsigned int i = 0; i < k; ++i)
    {
        r *= (n - i);
        r /= (i + 1);
    }
    return r;
}

unsigned int pow6(unsigned int n)
{
    assert(n > 0);
    pow6_eval:;
    switch (n)
    {
        case 0: return 1;
        case 1: return 6;
        case 2: return 36;
        case 3: return 216;
        case 4: return 1296;
        case 5: return 7776;
        case 6: return 46656;
        case 7: return 279936;
        case 8: return 1679616;
        default: break;
    }

    unsigned int r = 1;
    do {
        n -= 8;
        r *= 1679616;
    } while (n > 8);
    goto pow6_eval;
}

// Probability mass function for the sum of N dice
float pmf(const unsigned int n, const unsigned int x)
{
    assert(n > 0);
    assert(n <= x);
    assert(x <= 6*n);

    const unsigned int k = (x - n)/6;

    float p = 0.0f;
    for (unsigned int i = 0; i <= k; ++i)
    {
        const unsigned int sign = (i % 2 == 0) ? 1 : -1;
        const unsigned int coeff = binom(n, i);
        const unsigned int term = binom(x-6*i-1, n - 1);
        p += sign * coeff * term;
    }
    p /= (float)pow6(n);
    return p;
};
} // namespace dice::prob