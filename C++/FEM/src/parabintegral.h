#ifndef PARABINTEGRAL_H
#define PARABINTEGRAL_H

#include "integral.h"

class ParabIntegral : public Integral
{
public:
    ParabIntegral();
    double integrate(std::function<double (double)> f, const int a, const int b, const int n);
};

#endif // PARABINTEGRAL_H
