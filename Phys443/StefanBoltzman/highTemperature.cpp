#include "functions.h"
using namespace std;

void highTemperature()
{
    // Initialize:
    gStyle->SetOptFit(1);
    auto *linefit = new TF1("linefit", "[0]*x + [1]");
    auto *expfit = new TF1("expfit", "[1]*TMath::Power(x,[0])");
    auto *graph1 = new TGraphErrors();
    auto *graph2 = new TGraphErrors();

    linefit->SetParNames("Slope", "Intercept");
    expfit->SetParNames("Power", "Scale", "Intercept");
    graph1->SetTitle("Log(Voltage) vs Log(Temperature) ;Log(Temperature(K)) ;Log(Voltage(V))");
    graph2->SetTitle("Voltage vs Temperature ;Temperature(K);Voltage(V)");

    vector<double> signal_V = {3.0, 2.6, 2.2, 2.0, 1.8, 1.6, 1.2, 0.8, 0.6, 0.4};
    vector<double> bulb_V = {8.41, 7.39, 6.95, 6.06, 5.88, 5.21, 4.11, 2.843, 2.474, 0.645};
    vector<double> bulb_I = {2.487, 2.323, 2.255, 2.099, 2.069, 1.942, 1.725, 1.440, 1.368, 0.857};

    double signal_V_error = 0.2;
    double offset = 47.6;
    double amp = pow(10, 4);
    double distance = 56.7 - offset;
    double R_ref = 0.3045;
    double R_ref_error = 0.0085;
    double T_ref = 300;
    double substract = 3.461e-5; //Give 0 for the real values.

    // Fill Graph1 and Graph2:
    for (size_t i = 0; i < bulb_V.size(); i++)
    {
        double V_true = signal_V[i] / amp;
        double V = bulb_V[i] / amp;
        double I = bulb_I[i] / amp;
        double R = V / I;

        double V_true_error = signal_V_error / amp;
        double V_error = calculateVoltUncertainty(bulb_V[i]) / amp;
        double I_error = calculateAmpUncertainty(bulb_I[i]) / amp;
        double R_error = sqrt(pow(V_error / I, 2) + pow(V * I_error / (I * I), 2));

        double Temperature = 242.85 + 170.2 * (R / R_ref);
        double Temperature_error = 156.2 * sqrt(pow(R_error / R_ref, 2) + pow(R * R_ref_error / (R_ref * R_ref), 2));

        graph1->SetPoint(i, log(Temperature), log(V_true-substract));
        graph1->SetPointError(i, calculateLogError(Temperature, Temperature_error), calculateLogError(V_true, V_true_error));

        graph2->SetPoint(i, Temperature, V_true-substract);
        graph2->SetPointError(i, Temperature_error, V_true_error);

        cout << "R =";
        printResult(R,R_error,2);
        cout << "T =";
        printResult(Temperature,Temperature_error,2);
    }

    // Fit Graph1:
    //linefit->SetParLimits(0,3.5,4.5);
    linefit->SetRange(7,8);
    auto *c1 = new TCanvas("c1", "c1", 200, 10, 600, 400);
    c1->SetGrid();
    c1->Draw();
    graph1->Fit("linefit");
    graph1->Draw ("A*");

    // Print Result:
    double n = linefit->GetParameter(0);
    double n_error = linefit->GetParError(0);
    cout << "n = ";
    printResult(n, n_error, 2);

    // Fit Grah2:
    expfit->SetParameter(1, 1.70743068e-17);
    expfit->SetParameter(0, 4);
    auto *c2 = new TCanvas("c2", "c2", 200, 10, 600, 400);
    c2->SetGrid();
    c2->Draw();
    graph2->Fit("expfit");
    graph2->Draw("A*");

    // Print Result:
    n = expfit->GetParameter(0);
    n_error = expfit->GetParError(0);
    cout << "n = ";
    printResult(n, n_error, 2);
};