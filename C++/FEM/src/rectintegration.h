#ifndef RECTINTEGRATION_H
#define RECTINTEGRATION_H

#include "integral.h"

class RectIntegration : public Integral
{
public:
    RectIntegration();
    double integrate(std::function<double (double)> f, const int a, const int b, const int n);
};

#endif // RECTINTEGRATION_H
