#include "functions.h"
using namespace std;

void ohm()
{
    // Initialize:
    gStyle->SetOptFit(1);
    auto *graph1 = new TGraphErrors();
    auto *graph2 = new TGraphErrors();
    auto *linefit = new TF1("linefit", "[0]*x + [1]");    

    graph1->SetTitle("Voltage(V) vs Current(A);Current(A);Voltage(V)");
    graph2->SetTitle("Temperature (Kelvin) vs R/R_300;R/R_300;T(Kelvin)");
    linefit->SetParNames("Slope", "Intercept");    

    vector<double> current = {5, 10, 15, 20, 25, 30, 35, 40, 45};
    vector<double> volt = {1.3, 2.8, 4.3, 5.8, 7.3, 9.0, 10.3, 12.1, 13.7};
    double current_error = 1;

    // Fill Graph1:
    for (size_t i = 0; i < current.size(); i++)
    {
        double volt_true = volt[i] * 1e-3;
        double current_true = current[i] * 1e-3;
        double volt_error = calculateVoltUncertainty(volt[i]) * 1e-3;
        double current_error = calculateAmpUncertainty(current[i]) * 1e-3;
        graph1->SetPoint(i, current_true, volt_true);
        graph1->SetPointError(i, current_error, volt_error);
    }

    // Fit Graph1:
    auto *c1 = new TCanvas("c1", "c1", 200, 10, 600, 400);
    c1->SetGrid();
    c1->Draw();
    graph1->Fit("linefit");
    graph1->Draw("A*");

    // Print Results:
    double R = linefit->GetParameter(0);
    double R_error = linefit->GetParError(0);
    cout << "R = ";
    printResult(R, R_error, 2);

    // Find T(R):
    vector<double> table_R = {1.0, 1.43, 1.87, 2.34, 2.85, 3.36, 3.88, 4.41, 4.95, 5.48,
                              6.03, 6.58, 7.14, 7.71, 8.28, 8.86, 9.44, 10.03, 10.63, 11.24,
                              11.84, 12.46, 13.08, 13.72, 14.34, 14.99, 15.63, 16.29, 16.95,
                              17.62, 18.28, 18.97, 19.66};

    vector<double> table_T = {300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200,
                              1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200,
                              2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200,
                              3300, 3400, 3500};

    // Fill Graph:
    for (size_t i = 0; i < table_R.size(); i++)
    {
        graph2->SetPoint(i, table_R[i], table_T[i]);
        graph2->SetPointError(i, 0.01, 100);
    };

    // Fit Graph2:
    auto *c2 = new TCanvas("c2", "c2", 200, 10, 600, 400);
    c2->SetGrid();
    c2->Draw();
    graph2->Fit("linefit");
    graph2->Draw("A*");

    // Print Results:
    double slope = linefit->GetParameter(0);
    double slope_error = linefit->GetParError(0);
    double intercept = linefit->GetParameter(1);
    double intercept_error = linefit->GetParError(1);
    cout << "Slope = ";
    printResult(slope, slope_error, 2);
    cout << "Intercept = ";
    printResult(intercept, intercept_error, 2);

    //170.2x - 242.9
};