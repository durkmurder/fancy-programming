#include "rectintegration.h"

RectIntegration::RectIntegration()
{
}

double RectIntegration::integrate(std::function<double (double)> f, const int a, const int b, const int n)
{
    double result = 0;
    const double h = (b - a) / static_cast<double>(n);
    for(unsigned i = 0; i < n; ++i)
    {
        result += f(a + (i + 0.5) * h);
    }
    return result * h;
}
