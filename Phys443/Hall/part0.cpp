#include "functions.h"
using namespace std;

void part0()
{
    // Initialize:
    gStyle->SetOptFit(1);
    auto *linefit = new TF1("linefit", "[0]*x + [1]");
    auto *graph1 = new TGraphErrors();
    auto *graph2 = new TGraphErrors();

    linefit->SetParNames("Slope", "Intercept");
    graph1->SetTitle("Resistance Measurements - Voltage vs Current;Current (A);Voltage(V)");
    graph2->SetTitle("Reference Measurements - Hall Voltage vs Current;Current (A);Voltage(V)");

    vector<double> Probe_V_resistance = {0.431,0.659,0.872,1.111,
    1.311,1.538,1.754,1.978,2.202,2.417};// Volt
    vector<double> Probe_I_resistance = {10.000,15.000,20.000,25.000,
    30.000,35.000,40.000,45.000,50.000,55.000};// mA
    vector<double> Probe_V_bias = {3.80,6.90,10.10,13.50,17.20,
    20.40,24.30,27.20,31.10,34.40,38.10,41.70};//mV
    vector<double> Probe_I_bias = {5,10,15,20,25,
    30,35,40,45,50,55,60};//mA

    double I_error = 1e-3;
    double convert = 1e-3;

    // Fill Graph1:
    for (size_t i = 0; i < Probe_V_resistance.size(); i++)
    {
        double V = Probe_V_resistance[i];
        double I = Probe_I_resistance[i] * convert;

        double V_error = calculateVoltUncertainty(V);

        graph1->SetPoint(i, I, V);
        graph1->SetPointError(i, I_error, V_error);
    }

    // Fill Graph2:
    for (size_t i = 0; i < Probe_V_bias.size(); i++)
    {
        double V = Probe_V_bias[i] * convert;
        double I = Probe_I_bias[i] * convert;

        double V_error = calculateVoltUncertainty(Probe_V_bias[i])*convert;

        graph2->SetPoint(i, I, V);
        graph2->SetPointError(i, I_error, V_error);
    }

    // Fit Graph1:
    linefit->SetRange(0,1);
    auto *c1 = new TCanvas("c1", "c1", 200, 10, 600, 400);
    c1->SetGrid();
    c1->Draw();
    graph1->Fit("linefit");
    graph1->Draw ("A*");

    // Print Result:
    double R = linefit->GetParameter(0);
    double R_error = linefit->GetParError(0);
    cout << "R = ";
    printResult(R, R_error, 3);

    // Fit Grah2:
    linefit->SetRange(0,1);
    auto *c2 = new TCanvas("c2", "c2", 200, 10, 600, 400);
    c2->SetGrid();
    c2->Draw();
    graph2->Fit("linefit");
    graph2->Draw("A*");

    // Print Result:
    double slope = linefit->GetParameter(0);
    double intercept = linefit->GetParameter(1);
    double slope_error = linefit->GetParError(0);
    double intercept_error = linefit->GetParError(1);
    cout << "Slope = ";
    printResult(slope, slope_error, 3);
    cout << "Intercept = ";
    printResult(intercept, intercept_error, 3);
};