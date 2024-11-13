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

void print_results(double true_value, double value, double error){
      double relative_error = abs(true_value - value) * 100 / true_value;
      double sigma = abs(true_value - value) / error;
      cout << value << " +- " << error;
      cout << ", Sigma = " << significant_round(sigma,2);
      cout << ", Relative Error = " << significant_round(relative_error,2) << "%" << endl;
}

double calculateLambdaError(double n, double L, double y_n, double d, double sigma_d, double sigma_y, double sigma_L) {
    // Partial derivatives
    double partial_lambda_d = y_n / (n * sqrt(4 * pow(L, 2) + pow(y_n, 2)));
    double partial_lambda_y = y_n * d / (n * sqrt(4 * pow(L, 2) + pow(y_n, 2)));
    double partial_lambda_L = -4 * d * L * y_n / (n * pow(4 * pow(L, 2) + pow(y_n, 2), 1.5));

    // Error calculation
    double sigma_lambda = sqrt(pow(partial_lambda_d * sigma_d, 2) + pow(partial_lambda_y * sigma_y, 2) + pow(partial_lambda_L * sigma_L, 2));

    return sigma_lambda;
}

double calculateSeparationError(double n, double L, double y_n, double lambda, double sigma_lambda, double sigma_y, double sigma_L) {
    // Partial derivatives
    double partial_d_lambda = n * sqrt(4 * pow(L, 2) + pow(y_n, 2)) / y_n;
    double partial_d_y = -4 * lambda * pow(L, 2) * n / (pow(y_n, 2) * sqrt(4 * pow(L, 2) + pow(y_n, 2)));
    double partial_d_L = 4 * lambda * L * n / (y_n * sqrt(4 * pow(L, 2) + pow(y_n, 2)));

    // Error calculation
    double sigma_d = sqrt(pow(partial_d_lambda * sigma_lambda, 2) + pow(partial_d_y * sigma_y, 2) + pow(partial_d_L * sigma_L, 2));

    return sigma_d;
}

void find_lambda(vector<double> measure, double L, double d,
                 double measure_error, double L_error, double d_error)
{
   double average;
   double average_error_squared;
   int n = measure.size();
   for (int i = 0; i < n; i++)
   {
      double y_n = measure[i];
      double sin_theta = y_n / sqrt(4 * pow(L, 2) + pow(y_n, 2));
      double lambda = d * sin_theta / (i + 1);
      
      double error = calculateLambdaError((i+1), L, y_n, d, d_error, measure_error, L_error);
      cout << "n = " << i+1 << ", Value = ";
      print_results(650, significant_round(lambda*1e9,2) , significant_round(error*1e9,2));
      average += (lambda / n);
      average_error_squared += pow(error / n, 2);
   }
   double error = sqrt(average_error_squared);
   cout << "Average = ";
   print_results(650, significant_round(average*1e9,2), significant_round(error*1e9,2));
}

void find_separation(vector<double> measure, double L, double lambda,
                     double measure_error, double L_error, double lambda_error)
{
   double average;
   double average_error_squared;
   int n = measure.size();
   for (int i = 0; i < n; i++)
   {
      double y_n = measure[i];
      double sin_theta = y_n / sqrt(4 * pow(L, 2) + pow(y_n, 2));
      double separation = (i + 1) * lambda / sin_theta;

      double error = calculateSeparationError((i+1),L,y_n,lambda,lambda_error,measure_error,L_error);
      cout << "n = " << i+1 << ", Value = ";
      print_results(0.16,significant_round(separation*1e3,2) , significant_round(error*1e3,2));

      average += (separation / n);
      average_error_squared += pow(error/n, 2);
   }

   double error = sqrt(average_error_squared);
   cout << "Average = ";
   print_results(0.16, significant_round(average*1e3,2), significant_round(error*1e3,2));
}

void slit()
{
   double distance = 152 * 1e-2;
   double distance_error = 1 * 1e-2;
   double lambda = 650 * 1e-9;
   double lambda_error = 1 * 1e-9;
   double measure_error = 1 * 1e-3;
   double slit_error = 0.01 * 1e-3;
   vector<double> single_slit = {0.013, 0.025, 0.038, 0.049};
   vector<double> double_slit_25 = {0.008, 0.016, 0.025, 0.032};
   vector<double> double_slit_50 = {0.004, 0.008, 0.012, 0.016};

   cout << "Double Slit with 0.25mm separation" << endl;
   find_lambda(double_slit_25, distance, 0.25 * 1e-3, measure_error, distance_error, slit_error);
   cout << "\nDouble Slit with 0.50mm separation" << endl;
   find_lambda(double_slit_50, distance, 0.50 * 1e-3, measure_error, distance_error, slit_error);
   cout << "\nSingle Slit with 0.16mm separation" << endl;
   find_separation(single_slit, distance, lambda, measure_error, distance_error, lambda_error);
}