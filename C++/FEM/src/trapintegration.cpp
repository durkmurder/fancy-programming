#include "trapintegration.h"

TrapIntegration::TrapIntegration()
{
}

double TrapIntegration::integrate(std::function<double (double)> f, const int a, const int b, const int n)
{
    double result = 0;
    const double h = (b - a) / static_cast<double>(n);
    for(unsigned i = 1; i < n; ++i)
    {
        result += f(a + i * h);
    }
    return h / 2 * (f(a) + f(b)) + h * result;
}
