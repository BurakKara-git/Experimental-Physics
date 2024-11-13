#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

double refraction(double d, double lambda, double m, double theta)
{
    double up = (d - (m * lambda / 2)) * (1 - cos(theta));
    double down = d * (1 - cos(theta)) - (m * lambda / 2);
    return up / down;
}

double find_error(double d, double l, double m, double t,
             double d_error, double lambda_error, double m_error, double theta_error)
{
    double partial_R_d = ((1 - cos(t)) * (d * (1 - cos(t)) + m * -1 / 2 * l) - pow(1 - cos(t), 2) * (d + m * -1 / 2 * l)) / pow(d * (1 - cos(t)) + m * -1 / 2 * l, 2);
    double partial_R_m = (l * -1 / 2 * (1 - cos(t)) * (d * (1 - cos(t)) + m * -1 / 2 * l) + (d + m * -1 / 2 * l) / 2 * (1 - cos(t)) * l) / pow(d * (1 - cos(t)) + m * -1 / 2 * l, 2);
    double partial_R_l = (m * -1 / 2 * (1 - cos(t)) * (d * (1 - cos(t)) + m * -1 / 2 * l) + (d + m * -1 / 2 * l) / 2 * (1 - cos(t)) * m) / pow(d * (1 - cos(t)) + m * -1 / 2 * l, 2);
    double partial_R_t = ((d + m * -1 / 2 * l) * sin(t) * (d * (1 - cos(t)) + m * -1 / 2 * l) - (d + m * -1 / 2 * l) * (1 - cos(t)) * d * sin(t)) / pow(d * (1 - cos(t)) + m * -1 / 2 * l, 2);

    double delta_R = sqrt(pow(partial_R_d * d_error, 2) + pow(partial_R_m * m_error, 2) + pow(partial_R_l * lambda_error, 2) + pow(partial_R_t * theta_error, 2));
    return delta_R;
}

double significant_round(double value, int digits)
{
   if (value == 0.0)
        return 0.0;
    double factor = pow(10.0, digits - ceil(log10(fabs(value))));
    return round(value * factor) / factor;   
}

void print_results(double true_value, double value, double error){
      double relative_error = abs(true_value - value) * 100 / true_value;
      double sigma = abs(true_value - value) / error;
      cout << value << " +- " << error;
      cout << ", Sigma = " << significant_round(sigma,2);
      cout << ", Relative Error = " << significant_round(relative_error,2) << "%" << endl;
}

void interferometer()
{
    double d = 0.55 * 1e-2;
    double d_error = 0.01 * 1e-2;
    double theta_error = 0.1;
    double lambda = 632.8 * 1e-9;
    double lambda_error = 0.1 * 1e-9;
    double m_error = sqrt(10);
    vector<int> m_vector = {20, 30, 40, 50, 60, 70, 80, 90, 100, 110};
    vector<double> theta_vector = {4.7, 6.4, 7.3, 8.2, 8.7, 9.4, 9.8, 10.5, 10.7, 11.1};

    double average = 0;
    double average_error_squared = 0;
    int num = m_vector.size();
    for (size_t i = 0; i < num; i++)
    {
        double n = refraction(d, lambda, m_vector[i], theta_vector[i] * M_PI / 180);
        double error = find_error(d, lambda, m_vector[i], theta_vector[i] * M_PI / 180,
        d_error, lambda_error, m_error, theta_error * M_PI / 180);
        cout << "Count = " << m_vector[i] <<", n = ";
        print_results(1.515,significant_round(n,2),significant_round(error,2));

        average += n / num;
        average_error_squared += pow(error/num, 2);
    }

    double error = sqrt(average_error_squared);

    cout << "Average, n = ";
    print_results(1.515, significant_round(average,2),significant_round(error,2));
}