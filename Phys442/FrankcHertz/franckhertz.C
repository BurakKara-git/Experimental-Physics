#include <iostream>
#include <fstream>
#include <string>
{
    gStyle->SetOptFit(1);
    const int n_peak = 10;
    double *delta_E, *sd_delta_E;
    delta_E = new double[n_peak];
    sd_delta_E = new double[n_peak];
    string file_names[] = {"u1-0.46_u3-0.72_t-180.csv",
                           "u1-0.52_u3-0.72_t-180.csv",
                           "u1-0.57_u3-0.72_t-180.csv",
                           "u1-0.60_u3-1.33_t-180.csv",
                           "u1-0.60_u3-1.33_t-185.csv",
                           "u1-0.60_u3-1.33_t-195.csv",
                           "u1-0.60_u3-1.33_t-202.csv",
                           "u1-0.65_u3-0.28_t-180.csv",
                           "u1-0.65_u3-0.72_t-180.csv",
                           "u1-0.65_u3-1.90_t-180.csv"};
    const string DELIMITER = "_";
    string u1, u3, t;
    int count = 0;
    for (string file_name : file_names) {
        double gauss_mean[4] = {0,0,0,0};
        double sd_gauss_mean[4] = {0,0,0,0};
        double gauss_sigma[4] = {0,0,0,0};
        double sd_gauss_sigma[4] = {0,0,0,0};
        double delta_peak[3] = {0,0,0};
        double sd_delta_peak[3] = {0,0,0};

        string str1 = "Franck_Hertz-Data/";
        string str2 = str1 + file_name;
        const char *path = str2.c_str();

        int pos1 = file_name.find(DELIMITER);
        int pos2 = file_name.find(DELIMITER, pos1 + 1);
        int pos3 = file_name.find(".csv");
        u1 = file_name.substr(2, pos1 - 2);
        u3 = file_name.substr(pos1 + 3, pos2 - pos1 - 3);
        t = file_name.substr(pos2 + 2, pos3 - pos2 - 2);
        
        string title_1 = "#splitline{#scale[0.8]{Collector Current vs Acceleration Voltage}}{#scale[0.5]";
        string title_2 = "{U_1" + u1 + ", U_3" + u3 + ", T" + t + "}}";
        string title_3 = title_1 + title_2 + ";Acceleration Voltage (V); Collector Current(nA)";        
        string str4 = file_name + ".pdf";

        const char *save = str4.c_str();
        const char *title = title_3.c_str();
        TTree *tree = new TTree("tree", "tree");
        tree->ReadFile(path);
        float I, U;
        tree->SetBranchAddress("U", &U);
        tree->SetBranchAddress("I", &I);

        double *x, *y, *sd_x, *sd_y;
        int n = tree->GetEntries();
        x = new double[n];
        y = new double[n];
        sd_x = new double[n];
        sd_y = new double[n];

        for (int i = 0; tree->LoadTree(i) >= 0; i++){
  	        tree->GetEntry(i);
  	        x[i] = U;
            y[i] = I;
            sd_y[i] = 0.005;
            sd_x[i] = 0.015;
        }

        TCanvas *c1 = new TCanvas();

        TGraphErrors *graph = new TGraphErrors(n,x,y,sd_x,sd_y);
        graph->SetTitle(title);
        graph->SetMarkerStyle(20);        
        TF1 *f1 = new TF1("f1","gaus",9.5,11);
        TF1 *f2 = new TF1("f2","gaus",14.5,15.7);
        TF1 *f3 = new TF1("f3","gaus",19.5,20.7);
        TF1 *f4 = new TF1("f4","gaus",24.5,25.7);
        
        graph->Fit(f1, "QSR");
        graph->Fit(f2, "QSR+");
        graph->Fit(f3, "QSR+");
        graph->Fit(f4, "QSR+");

        graph->Draw("A*");
        graph->Draw();

        gauss_mean[0] = f1->GetParameter(1);
        gauss_mean[1] = f2->GetParameter(1);
        gauss_mean[2] = f3->GetParameter(1);
        gauss_mean[3] = f4->GetParameter(1);

        gauss_sigma[0] = f1->GetParameter(2);
        gauss_sigma[1] = f2->GetParameter(2);
        gauss_sigma[2] = f3->GetParameter(2);
        gauss_sigma[3] = f4->GetParameter(2);
        
        sd_gauss_mean[0] = f1->GetParError(1);
        sd_gauss_mean[1] = f2->GetParError(1);
        sd_gauss_mean[2] = f3->GetParError(1);
        sd_gauss_mean[3] = f4->GetParError(1);

        sd_gauss_sigma[0] = f1->GetParError(1);
        sd_gauss_sigma[1] = f2->GetParError(1);
        sd_gauss_sigma[2] = f3->GetParError(1);
        sd_gauss_sigma[3] = f4->GetParError(1);

        for(int i = 0; i < 3; i++){
            delta_peak[i] = gauss_mean[i+1] - gauss_mean[i];
            sd_delta_peak[i] = sqrt(pow(gauss_sigma[i+1],2) + pow(gauss_sigma[i],2));
            cout << "Delta_E_" << i+1 << " for " << file_name  << " : " <<
            delta_peak[i] << " +- " << sd_delta_peak[i] << " V" << endl;
        }

        double weight_peak = 0, weight_sd_peak = 0;
        double totw_peak = 0, totw_sd_peak;
        double delta_peak_bar = 0, sd_delta_peak_bar = 0;        

        for (int i=0; i<3; ++i) {
            weight_peak = 1./(sd_gauss_mean[i]*sd_gauss_mean[i]);
            weight_sd_peak = 1./(sd_gauss_sigma[i]*sd_gauss_sigma[i]);
            totw_peak += weight_peak;
            totw_sd_peak += weight_sd_peak;
            delta_peak_bar += delta_peak[i] * weight_peak;
            sd_delta_peak_bar += sd_delta_peak[i] * weight_sd_peak;
        }

        delta_peak_bar /= totw_peak;
        sd_delta_peak_bar /= totw_sd_peak;
        
        delta_E[count] = delta_peak_bar;
        sd_delta_E[count] = sd_delta_peak_bar;
        count += 1;
        c1->Print(save);
    }

    TCanvas *c2 = new TCanvas();
    TH1D *h1 = new TH1D("Parameters","Average Voltages - Gauss Fit",26,4.5,5.5); 
    for (int i=0; i<10; i++) h1->Fill(delta_E[i] ,1./(pow(sd_delta_E[i],2)));
    h1->SetMarkerStyle(kFullCircle);
    h1->GetXaxis()->SetTitle("Average P-P Voltage (V)");
    h1->GetYaxis()->SetTitle("Weighted Occurence");
    TF1 *f_gauss = new TF1("f_gauss","gaus",4.85,5);
    f_gauss->SetParameters(h1->GetMaximum(), h1->GetMean(), h1->GetRMS() );
    h1->Fit("f_gauss", "Q");
    h1->Draw("HIST same E");

    
    double result = f_gauss->GetParameter(1);
    double sd_result = f_gauss->GetParameter(2);
        for(int i = 0; i < 10; i++){
        cout << delta_E[i] << " +- " << sd_delta_E[i] << endl;
    } 

    cout << "First Excitation Energy = " << result << " +- " 
    << sd_result << " eV" <<endl;
    c2->Print("Franck-Hertz.pdf");
    return 0;
} 