#ifndef functions
#define functions

#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

void printResult(double val, double error, int sf)
{
    int factor = sf - ceil(log10(fabs(error)));
    if (factor <= 0)
    {        
        cout << setprecision(sf) << val;
        cout << " +- " << error << endl;
    }
    else
    {
        cout << fixed << setprecision(factor) << val;
        cout << " +- " << error << endl;
    }
}

double calculateVoltUncertainty(double V)
{
    V = abs(V);
    double error;
    if (V <= 0.4)
    {
        error = (3 * V / 100) + 0.4;
    }
    else if (V <= 4)
    {
        error = (5 * V / 100) + 0.003;
    }
    else if (V <= 40)
    {
        error = (5 * V / 100) + 0.03;
    }
    else if (V <= 400)
    {
        error = (5 * V / 100) + 0.3;
    }
    else
    {
        error = (V / 100) + 4;
    }
    return error;
};


double calculateAmpUncertainty(double A)
{
    A = abs(A);
    return (2 * A / 100) + 0.005;
};


double calculateLogError(double val, double sigma)
{
    return sigma / val;
};
#endif