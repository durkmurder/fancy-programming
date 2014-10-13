#ifndef TRAPINTEGRATION_H
#define TRAPINTEGRATION_H

#include "integral.h"

class TrapIntegration : public Integral
{
public:
    TrapIntegration();
    double integrate(std::function<double (double)> f, const int a, const int b, const int n);
};

#endif // TRAPINTEGRATION_H
