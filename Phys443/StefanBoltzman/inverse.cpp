#include "functions.h"
using namespace std;

void inverse()
{
    vector<double> distance = {90.3, 81.2, 74.5, 70.4, 68.3, 66.5, 65.2, 64.0, 63.1,
                               62.4, 61.7, 61.2, 60.1, 59.8};

    vector<double> signal_V = {0.20, 0.40, 0.60, 0.80, 1.00, 1.20, 1.40, 1.60, 1.80, 2.00,
                               2.20, 2.40, 2.60, 2.80};

    double signal_V_error = 0.2;
    double distance_error = 0.2;
    double offset = 47.6;
    double amp = pow(10, 5);

    gStyle->SetOptFit(1);
    auto *linefit = new TF1("linefit", "[0]*x + [1]");
    linefit->SetParName(0, "Slope");
    linefit->SetParName(1, "Intercept");
    auto *graph1 = new TGraphErrors();
    auto *graph2 = new TGraphErrors();
    graph1->SetTitle("Log(Voltage) vs Log(Distance); Log(Distance(cm)); Log(Voltage(V))");
    graph2->SetTitle("Voltage vs Distance; Distance(cm); Voltage(V)");

    for (size_t i = 0; i < distance.size(); i++)
    {
        double distance_true = distance[i] - offset;
        double V_true = signal_V[i] / amp;
        double V_true_error = signal_V_error / amp;
        graph1->SetPoint(i, log(distance_true), log(V_true));
        graph1->SetPointError(i, calculateLogError(distance_true, distance_error), calculateLogError(V_true, V_true_error));
        graph2->SetPoint(i, distance_true, V_true);
        graph2->SetPointError(i, distance_error, V_true_error);
    }

    graph1->Fit("linefit");
    graph1->Draw("A*");

    double n = linefit->GetParameter(0);
    double n_error = linefit->GetParError(0);

    cout << "Power = ";
    printResult(n, n_error, 2);

    auto *expfit = new TF1("expfit", "[1]*TMath::Power(x,[0])");
    expfit->SetParNames("Power", "Scale");

    auto *c2 = new TCanvas("c2", "c2", 200, 10, 600, 400);
    c2->SetGrid();
    c2->Draw();
    graph2->Fit("expfit");
    graph2->Draw("A*");

    n = expfit->GetParameter(0);
    n_error = expfit->GetParError(0);
    cout << "Power = ";
    printResult(n, n_error, 2);
};