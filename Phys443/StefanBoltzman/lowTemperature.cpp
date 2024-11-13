#include "functions.h"
using namespace std;

void lowTemperature()
{
    // Initialize:
    gStyle->SetOptFit(1);
    auto *linefit = new TF1("linefit", "[0]*x + [1]");
    auto *graph1 = new TGraphErrors();
    auto *graph2 = new TGraphErrors();

    linefit->SetParNames("Slope", "Intercept");
    graph1->SetTitle("Log(Voltage) vs Log(Temperature) ;Log(Temperature(K)) ;Log(Voltage(V))");
    graph2->SetTitle("Voltage vs Temperature ;Temperature(K);Voltage(V)");

    vector<double> V = {
        0.30, 1.30, 1.70, 2.20, 2.70, 3.00, 3.30, 3.40, 3.50, 3.60,
        3.70, 3.80, 3.90, 4.00, 4.10, 4.20, 4.30, 4.40, 4.50, 4.60,
        4.70, 4.80, 4.90, 5.00, 5.10, 5.20, 5.30, 5.40, 5.50, 5.60,
        5.70, 5.80, 5.90, 6.00, 6.10, 6.20, 6.30, 6.40, 6.50, 6.60,
        6.70, 6.80, 6.90, 7.00, 7.10, 7.20, 7.30, 7.40, 7.50, 7.60,
        7.70, 7.80, 7.90, 8.00, 8.10, 8.20, 8.30, 8.40, 8.50, 8.60,
        8.70, 8.80, 8.90, 9.00, 9.10};

    vector<double> T = {
        55.70, 131.40, 149.10, 174.80, 196.40, 210.40, 219.80, 226.10,
        227.60, 230.70, 234.20, 237.70, 241.60, 244.80, 247.00, 251.30,
        253.40, 257.50, 261.60, 263.70, 265.90, 269.20, 271.50, 274.20,
        276.80, 279.60, 282.60, 285.00, 288.20, 291.30, 294.20, 295.90,
        298.80, 300.60, 303.60, 305.70, 308.60, 310.80, 313.40, 315.60,
        318.20, 319.70, 322.40, 325.00, 327.20, 329.20, 331.40, 333.60,
        335.80, 338.00, 339.80, 342.20, 344.40, 346.20, 348.60, 350.20,
        352.80, 354.20, 356.70, 358.60, 361.00, 362.50, 364.90, 366.40,
        368.40};

    double T_error = 0.1;

    // Fill Graph1 and Graph2:
    for (size_t i = 0; i < V.size(); i++)
    {
        double logV_error = calculateLogError(V[i] * pow(10, -3), calculateVoltUncertainty(V[i]) * pow(10, -3));
        double logT_error = calculateLogError(T[i] + 273.2, T_error);
        graph1->SetPoint(i, log(T[i] + 273.2), log(V[i] * pow(10, -3)));
        graph1->SetPointError(i, logT_error, logV_error);

        double V_error = calculateVoltUncertainty(V[i]) * pow(10, -3);
        graph2->SetPoint(i, T[i] + 273.2, V[i] * pow(10, -3));
        graph2->SetPointError(i, T_error, V_error);
    }

    // Fit Graph1:
    auto *c1 = new TCanvas("c1", "c1", 200, 10, 600, 400);
    c1->SetGrid();
    c1->Draw();
    graph1->Fit("linefit");
    graph1->Draw("A*");

    // Print Result:
    double n = linefit->GetParameter(0);
    double n_error = linefit->GetParError(0);
    cout << "n = ";
    printResult(n, n_error, 2);

    // Fit Graph2:
    string fitarg = "[1]*TMath::Power(x,[0])";
    cout << fitarg << endl;
    auto *expfit = new TF1("expfit", fitarg.c_str());
    expfit->SetParameter(0,1.70743068e-17);
    expfit->SetParNames("Power", "Scale");
    auto *c2 = new TCanvas("c2", "c2", 200, 10, 600, 400);
    c2->SetGrid();
    c2->Draw();
    graph2->Fit("expfit");
    graph2->Draw("A*");
    n = expfit->GetParameter(0);
    n_error = expfit->GetParError(0);
    cout << "n = ";
    printResult(n, n_error, 2);
    
};