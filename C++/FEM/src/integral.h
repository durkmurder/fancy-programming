#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <functional>

class Integral
{
public:
    Integral();
    double virtual integrate(std::function<double(double)> f, const int a, const int b, const int n) =0;
};

#endif // INTEGRAL_H
