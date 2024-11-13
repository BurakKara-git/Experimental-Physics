#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

double significant_round(double value, int digits)
{
   if (value == 0.0)
        return 0.0;
    double factor = pow(10.0, digits - ceil(log10(fabs(value))));
    return round(value * factor) / factor;   
}

double find_error(double d, double r, double delta_d, double delta_r){
    double partial_R_d = (d * 8.0 / (2.0 * sqrt(pow(d * 2.0 / r, 2) + 1.0)) / pow(r, 2));
    double partial_R_r = -(pow(d, 2) * 8.0 / (2.0 * sqrt(pow(d * 2.0 / r, 2) + 1.0)) / pow(r, 3));
    double delta_R = sqrt(pow(partial_R_d * delta_d, 2) + pow(partial_R_r * delta_r, 2));

    return delta_R;
}

void print_results(double true_value, double value, double error){
      double relative_error = abs(true_value - value) * 100 / true_value;
      double sigma = abs(true_value - value) / error;
      cout << value << " +- " << error;
      cout << ", Sigma = " << significant_round(sigma,2);
      cout << ", Relative Error = " << significant_round(relative_error,2) << "%" << endl;
}

void pfund(){
    double d = 0.55 * 1e-2;
    double d_error = 0.01 * 1e-2;
    double r = 2.0 * 1e-2 / 2;
    double r_error = 0.2 * 1e-2 / 2;

    double n = sqrt (1+ pow(2*d/r,2));
    double error = find_error(d,r,d_error,r_error);
    cout << "n = ";
    print_results(1.515, significant_round(n,2), significant_round(error,2));
}