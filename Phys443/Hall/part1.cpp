#include "functions.h"
using namespace std;

void part1()
{
    // Initialize:
    gStyle->SetOptFit(1);
    auto *linefit = new TF1("linefit", "[0]*x + [1]");
    auto *graph1 = new TGraphErrors();
    auto *graph2 = new TGraphErrors();
    auto *graph3 = new TGraphErrors();

    linefit->SetParNames("Slope", "Intercept");
    graph1->SetTitle("Hall Voltage vs Magnetic Field - Positive Magnetic Field (Constant Probe Current); Magnetic Field(T); Voltage(V)");
    graph2->SetTitle("Hall Voltage vs Magnetic Field - Negative Magnetic Field (Constant Probe Current); Magnetic Field(T); Voltage(V)");
    graph3->SetTitle("Hall Voltage vs Current - With Offset (Constant Magnetic Field); Current(A); Voltage(V)");

    vector<double>  measure1_B = {18.2000,31.6000,46.3000,61.8000,
    76.3000,90.0000,104.5000,120.5000,
    135.4000,150.6000,165.3000,180.8000,};
    vector<double> measure1_V = {19.4000,15.9000,12.2000,8.4000,
    4.6000,1.1000,-2.6000,-6.5000,
    -10.2000,-14.1000,-17.7000,-21.6000};
    double measure1_I = 0.035;

    vector<double>  measure2_B = {-12.3000,-25.2000,-40.9000,-56.4000,
    -71.2000,-89.7000,-103.8000,-117.9000,
    -132.2000,-146.6000,-163.5000,-178.7000,};
    vector<double> measure2_V = {27.1000,30.4000,34.4000,38.3000,
    42.1000,46.8000,50.4000,53.8000,
    57.5000,61.2000,65.4000,69.2000};
    double measure2_I = 0.035;

    vector<double>  measure3_I = {5.0000,10.0000,15.0000,20.0000,
    25.0000,30.0000,35.0000,40.0000,
    45.0000,50.0000,55.0000,60.0000};
    vector<double> measure3_V = {7.4000,13.1000,20.2000,26.9000,
    33.2000,39.2000,46.7000,52.8000,
    59.2000,66.0000,72.9000,80.2000};
    double measure3_B = -88.5e-3;

    double B_error = 5/100;
    double I_error = 1e-3;
    double convert = 1e-3;
    double t = 1e-3; 
    double t_error = 0.2e-3;
    double L = 16e-3;
    double L_error = 4e-3;
    double w = 10e-3;
    double w_error = 0.2e-3;
    double A = w*t;
    double A_error = sqrt(pow(w_error*t,2)+pow(w*t_error,2));
    double R = 44.13;
    double R_error = 1.86;

    double sigma = L/(A*R);
    double sigma_error = sqrt(pow(L_error/(A*R),2)+pow(A_error*sigma/A,2)+pow(R_error*sigma/R,2));

    double bias_slope = 0.6852;
    double bias_slope_error = 0.0225;

    // Fill Graph1:
    for (size_t i = 0; i < measure1_V.size(); i++)
    {
        double V = measure1_V[i] * convert;
        double B = measure1_B[i] * convert;

        double V_error = calculateVoltUncertainty(measure1_V[i])*convert;
        double B_error_true = B*B_error;

        graph1->SetPoint(i, B, V);
        graph1->SetPointError(i, B_error_true, V_error);
    }

    // Fill Graph2:
    for (size_t i = 0; i < measure2_V.size(); i++)
    {
        double V = measure2_V[i] * convert;
        double B = measure2_B[i] * convert;

        double V_error = calculateVoltUncertainty(measure2_V[i])*convert;
        double B_error_true = B*B_error;

        graph2->SetPoint(i, B, V);
        graph2->SetPointError(i, B_error_true, V_error);
    }

    // Fill Graph3:
    for (size_t i = 0; i < measure3_V.size(); i++)
    {
        double V = measure3_V[i] * convert;
        double I = measure3_I[i] * convert;

        double bias = bias_slope*I;
        V -= bias;

        double V_error_temp = calculateVoltUncertainty(measure3_V[i])*convert;
        double V_error = sqrt(pow(V_error_temp,2)+pow(I_error*bias_slope,2)+pow(bias_slope_error*I,2));

        graph3->SetPoint(i, I, V);
        graph3->SetPointError(i, I_error, V_error);
    }

    // Fit Graph1:
    auto *c1 = new TCanvas("c1", "c1", 200, 10, 600, 400);
    c1->SetGrid();
    c1->Draw();
    graph1->Fit("linefit","Q");
    graph1->Draw ("A*");

    // Print Result:
    double slope = linefit->GetParameter(0);
    double intercept = linefit->GetParameter(1);
    double slope_error = linefit->GetParError(0);
    double intercept_error = linefit->GetParError(1);
    cout << "Positive Magnetic Field:" << endl;
    cout << "Slope = ";
    printResult(slope, slope_error, 3);
    cout << "Intercept = ";
    printResult(intercept, intercept_error, 3);

    // Calculate Hall Coefficient
    double R_H = slope*t/measure1_I;
    double R_H_error = sqrt(pow(slope_error*t/measure1_I,2) + pow(slope*t_error/measure1_I,2) + pow(I_error*slope*t/(pow(measure1_I,2)),2));
    cout << "R_H = ";
    printResult(R_H,R_H_error,3);

    double mobility = R_H*sigma;
    double mobility_error = sqrt(pow(R_H_error*sigma,2)+pow(R_H * sigma_error,2));
    cout << "Mobility = ";
    printResult(mobility,mobility_error,3);

    // Fit Graph2:
    auto *c2 = new TCanvas("c2", "c2", 200, 10, 600, 400);
    c2->SetGrid();
    c2->Draw();
    graph2->Fit("linefit","Q");
    graph2->Draw ("A*");
    
    // Print Result:
    slope = linefit->GetParameter(0);
    intercept = linefit->GetParameter(1);
    slope_error = linefit->GetParError(0);
    intercept_error = linefit->GetParError(1);
    cout << "Negative Magnetic Field:" << endl;
    cout << "Slope = ";
    printResult(slope, slope_error, 3);
    cout << "Intercept = ";
    printResult(intercept, intercept_error, 3);

    // Calculate Hall Coefficient
    R_H = slope*t/measure2_I;
    R_H_error = sqrt(pow(slope_error*t/measure2_I,2) + pow(slope*t_error/measure2_I,2) + pow(I_error*slope*t/(pow(measure2_I,2)),2));
    cout << "R_H = ";
    printResult(R_H,R_H_error,3);

    //Calculate Mobility
    mobility = R_H*sigma;
    mobility_error = sqrt(pow(R_H_error*sigma,2)+pow(R_H * sigma_error,2));
    cout << "Mobility = ";
    printResult(mobility,mobility_error,3);

    // Fit Graph3:
    auto *c3 = new TCanvas("c3", "c3", 200, 10, 600, 400);
    c3->SetGrid();
    c3->Draw();
    graph3->Fit("linefit","Q");
    graph3->Draw ("A*");
    
    // Print Result:
    slope = linefit->GetParameter(0);
    intercept = linefit->GetParameter(1);
    slope_error = linefit->GetParError(0);
    intercept_error = linefit->GetParError(1);
    cout << "Varying Current:" << endl;
    cout << "Slope = ";
    printResult(slope, slope_error, 3);
    cout << "Intercept = ";
    printResult(intercept, intercept_error, 3);

    // Calculate Hall Coefficient
    R_H = slope*t/measure3_B;
    double B_error_true = measure3_B*B_error;
    R_H_error = sqrt(pow(slope_error*t/measure3_B,2) + pow(slope*t_error/measure3_B,2) + pow(B_error_true*slope*t/(pow(measure3_B,2)),2));
    cout << "R_H = ";
    printResult(R_H,R_H_error,3);

    // Calculate Mobility
    mobility = R_H*sigma;
    mobility_error = sqrt(pow(R_H_error*sigma,2)+pow(R_H * sigma_error,2));
    cout << "Mobility = ";
    printResult(mobility,mobility_error,3);
};