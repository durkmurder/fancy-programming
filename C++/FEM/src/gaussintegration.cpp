#include "gaussintegration.h"

GaussIntegration::GaussIntegration()
{
}

double GaussIntegration::integrate(std::function<double (double)> f, const int a, const int b, const int n)
{
    const int points = 5;
    const double t[points] = { 0.90618, 0.538469, 0, -0.538469, -0.90618 };
    const double c[points] = { 0.23693, 0.47863, 0.56889, 0.47863, 0.23693 };

    double result = 0;
    for(unsigned i = 0; i < points; ++i)
    {
        result += c[i] * f( (b + a) / 2.0 + (b - a) / 2.0 * t[i]);
    }
    return result * (b - a) / 2.0;
}
