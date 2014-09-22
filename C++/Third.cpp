#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

const double epsilon = 0.00001;

double func(const double x)
{
    return x * cos(x);
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
        dy0[i] = mas[i + 1][0];
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

double backwardGauss(const double x, const int n, const std::vector<double> &xArray, const std::vector<double> &yArray, const double step)
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
        dy0[i] = mas[i + 1][n - i];
    }

    double res = dy0[0];

    std::vector<double> xn;
    xn.resize(n);
    xn[0] = x - mas[0][0];
    for(int i = 1 ; i < n; ++i)
    {
        if(i % 2 == 0)
            xn[i] = xn[i - 1] * (x + mas[0][i]);
        else
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
    std::vector<double> xArray, yArray;
    xArray.resize(2 * (n + 1));
    yArray.resize(2 * (n + 1));
    const double step = (b - a) / n;
    for(int i = 0; i < n + 1; ++i)
    {
        xArray[i] = (a + i * step);
        yArray[i] = func(xArray[i]);
    }
    //    std::cout << "STEP: " << step << std::endl;
    double nextX = a + (5) * step;
    std::cout << "Gold value: " << func(nextX) << std::endl
              << "Newton Interpolated value: " << forwardNewton(nextX, n, xArray, yArray, step) << std::endl;
    nextX = a + (n + 1) * step;
    std::cout << "Gold value: " << func(nextX) << std::endl
              << "Gauss Interpolated value: " << backwardGauss(nextX, n, xArray, yArray, step) << std::endl;
    return 0;
}

