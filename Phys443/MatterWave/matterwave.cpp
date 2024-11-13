#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

// Constants:
double R = 4.3 * 1e-2;
double L = 14.0 * 1e-2;
double h = 6.62607015 * 1e-34;  // exact
double q = 1.602176634 * 1e-19; // exact
double m = 9.1093837015 * 1e-31;

double V_error = 100;
double R_error = 0.1 * 1e-2;
double L_error = 0.3 * 1e-2;
double measure_error = 0.1 * 1e-2;

double significant_round(double value, int digits)
{
    if (value == 0.0)
        return 0.0;
    double factor = pow(10.0, digits - ceil(log10(fabs(value))));
    return round(value * factor) / factor;
}

double calculateTheta(double a)
{
    double numerator = R * sin(a / R);
    double denominator = L - R + R * cos(a / R);
    double result = 0.5 * atan2(numerator, denominator);
    return result;
}

double calculateThetaError(double a)
{
    double numerator_R = R * sin(a / R) * (2 * L - a * sin(a / R)) + 2 * a * (R - L) * cos(a / R) - a * R * pow(cos(a / R), 2);
    double denominator_R = R * pow(R * cos(a / R) + 2 * L - 2 * R, 2);
    double partial_R = numerator_R / denominator_R;

    double numerator_L = -2 * R * sin(a / R);
    double denominator_L = pow(R * cos(a / R) + 2 * L - 2 * R, 2);
    double partial_L = numerator_L / denominator_L;

    double numerator_a = 2 * (L - R) * cos(a / R) + R * pow(sin(a / R), 2) + R * pow(cos(a / R), 2);
    double denominator_a = pow(R * cos(a / R) + 2 * L - 2 * R, 2);
    double partial_a = numerator_a / denominator_a;

    double a_error = measure_error / 8;
    double error = sqrt(pow(partial_R * R_error, 2) + pow(partial_L * L_error, 2) + pow(partial_a * a_error, 2));

    return error;
}

double calculateWavelength(double V)
{
    return h / sqrt(2 * q * m * V);
}

double calculateWavelengthError(double V)
{
    double constant = h / sqrt(2 * q * m);
    return sqrt(pow(constant * -0.5 * pow(V, -1.5) * V_error, 2));
}

void print_results(double true_value, double value, double error)
{
    double relative_error = abs(true_value - value) * 100 / true_value;
    double sigma = abs(true_value - value) / error;
    cout << value << " +- " << error << " nm ";
    cout << ", True Value = " << true_value << " nm "; 
    cout << ", Sigma = " << significant_round(sigma, 3);
    cout << ", Relative Error = " << significant_round(relative_error, 3) << "%" << endl;
}

void matterwave()
{
    vector<double> V_vec = {5000, 4800, 4500, 4200, 3900, 3600, 3400, 2900, 2700, 2400};
    vector<vector<double>> Arc_in_matrix = {
        {0.021, 0.027, 0.020, 0.028},
        {0.021, 0.027, 0.022, 0.025},
        {0.022, 0.026, 0.021, 0.028},
        {0.022, 0.027, 0.023, 0.027},
        {0.022, 0.030, 0.024, 0.031},
        {0.024, 0.032, 0.025, 0.031},
        {0.026, 0.035, 0.025, 0.031},
        {0.025, 0.034, 0.024, 0.035},
        {0.028, 0.034, 0.028, 0.035},
        {0.029, 0.037, 0.028, 0.036}};

    vector<vector<double>> Arc_out_matrix = {
        {0.039, 0.045, 0.038, 0.045},
        {0.038, 0.043, 0.038, 0.043},
        {0.038, 0.044, 0.038, 0.044},
        {0.040, 0.046, 0.042, 0.047},
        {0.042, 0.051, 0.043, 0.051},
        {0.045, 0.052, 0.044, 0.053},
        {0.046, 0.055, 0.045, 0.054},
        {0.050, 0.060, 0.050, 0.060},
        {0.052, 0.060, 0.054, 0.062},
        {0.055, 0.063, 0.054, 0.065}};

    gStyle->SetOptFit(1);
    gErrorIgnoreLevel = kFatal;
    TF1 *fitFcn = new TF1("fitFcn", "[0]*x + [1]");
    fitFcn->SetParName(0, "Slope");
    fitFcn->SetParName(1, "Intercept");
    auto *graph1 = new TGraphErrors();
    graph1->GetXaxis()->SetTitle("Sin(#theta)");
    graph1->GetYaxis()->SetTitle("1/#sqrt{V}");

    auto *graph2 = new TGraphErrors();
    graph2->GetXaxis()->SetTitle("Sin(#theta)");
    graph2->GetYaxis()->SetTitle("1/#sqrt{V}");

    int row_num = Arc_in_matrix.size();
    for (int i = 0; i < row_num; i++)
    {
        int ncol = Arc_out_matrix[i].size();
        double sum1 = 0;
        double sum2 = 0;
        for (int j = 0; j < ncol; j++)
        {
            sum1 += Arc_in_matrix[i][j] / ncol;
            sum2 += Arc_out_matrix[i][j] / ncol;
        }
        double theta1 = calculateTheta(sum1 / 2);
        double theta2 = calculateTheta(sum2 / 2);
        double theta1_error = calculateThetaError(sum1 / 2);
        double theta2_error = calculateThetaError(sum2 / 2);
        double wavelength = calculateWavelength(V_vec[i]);
        double wavelength_error = calculateWavelengthError(V_vec[i]);
        cout << "V = " << V_vec[i] << " +- " << V_error << ", Lambda = " << significant_round(wavelength, 3) << " +- " << significant_round(wavelength_error, 3) << endl;
        double y_axis = 1 / (sqrt(V_vec[i]));
        double y_axis_error = wavelength_error / (h / sqrt(2 * q * m));
        double x1_axis_error = cos(theta1) * theta1_error;
        double x2_axis_error = cos(theta2) * theta2_error;
        graph1->SetPoint(i, sin(theta1), y_axis);
        graph1->SetPointError(i, x1_axis_error, y_axis_error);
        graph2->SetPoint(i, sin(theta2), y_axis);
        graph2->SetPointError(i, x2_axis_error, y_axis_error);
    }

    // First Ring
    TCanvas *c1 = new TCanvas("c1", "c1", 200, 10, 600, 400);
    c1->SetGrid();
    c1->Draw();
    graph1->Fit("fitFcn", "QR");
    string title = "1/#sqrt{V} vs. Sin(#theta) - First Ring";
    graph1->SetTitle(title.c_str());
    graph1->Draw("A*");
    double slope = fitFcn->GetParameter(0);
    double slope_error = fitFcn->GetParError(0);
    double d = slope * h / sqrt(8 * q * m);
    double d_error = slope_error * h / sqrt(8 * q * m);
    cout << "d10 = ";
    print_results(0.213, significant_round(d * 1e9, 3), significant_round(d_error * 1e9, 3));

    // Second Ring
    TCanvas *c2 = new TCanvas("c2", "c2", 200, 10, 600, 400);
    c2->SetGrid();
    c2->Draw();
    graph2->Fit("fitFcn", "QR");
    title = "1/#sqrt{V} vs. Sin(#theta) - Second Ring";
    graph2->SetTitle(title.c_str());
    graph2->Draw("A*");
    slope = fitFcn->GetParameter(0);
    slope_error = fitFcn->GetParError(0);
    d = slope * h / sqrt(8 * q * m);
    d_error = slope_error * h / sqrt(8 * q * m);
    cout << "d11 = ";
    print_results(0.123, significant_round(d * 1e9, 3), significant_round(d_error * 1e9, 3));

    TCanvas *c3 = new TCanvas("c3", "c3", 200, 10, 600, 400);
    c3->SetGrid();
    c3->Draw();
    TMultiGraph *mg = new TMultiGraph();
    mg->Add(graph1);
    mg->Add(graph2);
    mg->GetXaxis()->SetTitle("Sin(#theta)");
    mg->GetYaxis()->SetTitle("1/#sqrt{V}");
    title = "1/#sqrt{V} vs. Sin(#theta)";
    mg->GetHistogram()->SetTitle(title.c_str());
    mg->Draw("AP");
}