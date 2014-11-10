#include "parabintegral.h"

ParabIntegral::ParabIntegral()
{
}

double ParabIntegral::integrate(std::function<double (double)> f, const int a, const int b, const int n)
{
    double sum = 0;
    double result = 0;
    const double h = (b - a) / static_cast<double>(n);
    for(unsigned i = 1; i < n + 1; ++i)
    {
        sum += f(a + (i - 0.5) * h);
    }
    for(unsigned i = 1; i < n; ++i)
    {
        result += f(a + i * h);
    }
    result = result * 2 + f(a) + f(b) + sum * 4;
    return result * h / 6.0;
}
