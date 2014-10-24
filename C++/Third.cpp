#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

const double epsilon = 0.00001;
const double M_PI = 3.14159265359;

typedef unsigned int uint;

double func(const double x)
{
    return x * cos(x);
}

double factorial(double nValue)
{
    if(nValue == 0)
        return 1.0;
    if(nValue == 1)
        return 1.0;

    double result = nValue;
    double result_next;
    double pc = nValue;
    do
    {
        result_next = result * (pc - 1);
        result = result_next;
        --pc;
    } while(pc > 2);
    nValue = result;
    return nValue;
}

double binomialCoefficient(double nValue, double nValue2)
{
    double result;
    if(nValue2 == 1)
        return nValue;

    result = (factorial(nValue)) / (factorial(nValue2) * factorial((nValue - nValue2)));
    nValue2 = result;

    return nValue2;
}

double delta(const int k, const std::vector<double> fArray, const int i)
{
    double sum = 0;
   // int sign = k % 2 == 0 ? 1 : -1;
    for(int j = 0; j < k; ++j)
    {
        if(i + j < fArray.size() - 1)
        {
            sum += pow(-1.0, k - j) * binomialCoefficient(j, k) * fArray[i + j];
        }
    }
    return sum;
}

double forwardNewton(const double x, const int n, const std::vector<double> &xArray, const std::vector<double> &yArray, const double step)
{
    std::vector<std::vector<double> > mas;
    mas.resize(n + 2);
    for(int i = 0; i < n + 2; ++i)
        mas[i].resize(n + 1);

    mas[0].assign(xArray.begin(), xArray.end());
    mas[1].assign(yArray.begin(), yArray.end());

    int m = n;
    for(int i = 2; i < n + 2; ++i)
    {
        for(int j = 0; j < m; ++j)
        {
            mas[i][j] = mas[i - 1][j + 1] - mas[i - 1][j];
        }
        --m;
    }
    std::vector<double> dy0;
    dy0.resize(n + 1);
    for(int i = 0; i < n + 1; ++i)
    {
        dy0[i] = delta(i + 1, yArray, 0);//mas[i + 1][0];
    }

    double res = dy0[0];

    std::vector<double> xn;
    xn.resize(n);
    xn[0] = x - mas[0][0];
    for(int i = 1 ; i < n; ++i)
    {
        xn[i] = xn[i - 1] * (x - mas[0][i]);
    }

    uint fact = 1;
    for(int i = 1; i < n + 1; ++i)
    {
        fact *= i;
        res += (dy0[i] * xn[i - 1]) / (fact * pow(step, i));
    }
    return res;

}

double backwardGauss(const double x, const int n, const std::vector<double>  &xArray, const std::vector<double> &yArray, const double step)
{
    std::vector<std::vector<double> > mas;
    mas.resize(n + 2);
    for(int i = 0; i < n + 2; ++i)
        mas[i].resize(n + 1);

    mas[0].assign(xArray.begin(), xArray.end());
    mas[1].assign(yArray.begin(), yArray.end());

    int m = n;
    for(int i = 2; i < n + 2; ++i)
    {
        for(int j = 0; j < m; ++j)
        {
            mas[i][j] = mas[i - 1][j + 1] - mas[i - 1][j];
        }
        --m;
    }
    std::vector<double> dy0;
    dy0.resize(n + 1);
    for(int i = 0, k = 0; i < n + 1; ++i)
    {
        dy0[i] = mas[i + 1][k];
        if((i + 1) % 2 == 0)
        {
            ++k;
        }
    }

    double res = dy0[0];

    std::vector<double> xn;
    xn.resize(n);
    xn[0] = 1;
    for(int i = 1 ; i < n; ++i)
    {
        xn[i] = xn[i - 1] * (x - mas[0][i]);
    }

    double fact = 1;
    for(int i = 1; i < n + 1; ++i)
    {
        fact *= i;
        res += (dy0[i] * xn[i - 1]) / (fact * pow(step, i));
    }
    return res;
}


int main()
{

    const int n = 20;
    const double a = 0;
    const double b = M_PI / 4;
    double step = (b - a) / n;
    std::vector<double> xArray, yArray;
    xArray.resize(n + 1);
    yArray.resize(n + 1);

    for(int i = 0; i < n + 1; ++i)
    {
        xArray[i] = (a + i * step);
        yArray[i] = func(xArray[i]);
    }
    //    std::cout << "STEP: " << step << std::endl;
    double nextX = a + (5) * step;
    std::cout << "Gold value: " << func(nextX) << std::endl
              << "Newton Interpolated value: " << forwardNewton(nextX, n, xArray, yArray, step) << std::endl;



    /*
    std::vector<double> xArray, yArray;
    xArray.resize(2 * n + 1);
    yArray.resize(2 * n + 1);

    for(int i = 0, k = 0; i < 2 * n + 1; ++i)
    {

        xArray[i] = (i + 1) % 2 == 0 ? (a - k * step) : (a + k * step);
        yArray[i] = func(xArray[i]);
        if((i) % 2 == 0)
            ++k;
    }
    for(int i = 0; i < 2 * n + 1; i++)
                std::cout << xArray[i] << std::endl;
    double nextX = a + (n + 1) * step;

    std::cout << "Value to be interpolated: " << nextX << std::endl;
    std::cout << "Gold value: " << func(nextX) << std::endl
              << "Gauss Interpolated value: " << backwardGauss(nextX, 2 * n, xArray, yArray, step) << std::endl;
              */
    return 0;
}

