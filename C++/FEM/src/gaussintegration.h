#ifndef GAUSSINTEGRATION_H
#define GAUSSINTEGRATION_H

#include "integral.h"

class GaussIntegration : public Integral
{
public:
    GaussIntegration();
    double integrate(std::function<double (double)> f, const int a, const int b, const int n);
};

#endif // GAUSSINTEGRATION_H
