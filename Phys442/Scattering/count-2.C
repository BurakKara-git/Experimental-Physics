{
  //Initializing Variables
  float r_m = 0.057/2;
  float r_c = 0;
  float r_f = 0;
  
  //Initializing Arrays

  // First Case - no modifications
  const int n = 17;
  float I = 20 / (r_m*2 / 40);
  float sd_I = sqrt(pow(20/(r_m*r_m*2 / 40),2)*pow(0.001,2));
  float count[n] = {12,7,15,14,23,22,27,37,0,38,31,46,37,45,30,15,130};
  float angle[n] = {20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340};    
  float sin[n] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  float sx[n] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  float sy[n] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  

  //Filling Arrays and Histogram
  TH1F *h1 = new TH1F("h1","Count",n, 10, 350);  
    for (int i=0; i<n; i++){
      sin[i] = TMath::Sin(angle[i]*TMath::Pi()/360);
      sy[i] = sqrt(count[i]);
      for (int j=0; j<count[i]; j++)
        h1->Fill((20*i)+20);
    }

  //Ploting Histogram and Linear Fit
  gStyle->SetOptFit(1111);   
  TCanvas *c2 = new TCanvas();
  h1->SetTitle("Histogram of Counts");
  h1->GetXaxis()->SetTitle("Degree");
  h1->GetYaxis()->SetTitle("Count");   
  h1->Draw();

  TCanvas *c1 = new TCanvas();
  TGraphErrors *graph = new TGraphErrors(n,sin,count,sx,sy);
  TF1 *f1 = new TF1("f1", "[0]*x + [1]",0,1);
  f1->SetParameters(40,0);
  f1->SetParNames("Slope","Intercept");
  graph->SetTitle("Count vs Sin(Theta/2);Count;Sin(Theta/2)");
  graph->Draw("A*");
  graph->Fit("f1","EX0");

  //Calculating Radius
  float del_theta = 20*TMath::Pi()/180;
  float slope = f1->GetParameter(0);
  r_f = slope*2/ (I*del_theta);
  r_c = h1->GetEntries() / (2*I);

  //Calculating Errors
  float sd_n = sqrt(h1->GetEntries());
  float sd_slope = f1->GetParError(0);

  float sd_r_f = sqrt(pow(2/(I*del_theta),2) * pow(sd_slope,2) 
    + pow(2*slope/(I*I*del_theta),2)*pow(sd_I,2));
  float sd_r_c = sqrt(pow((1/(2*I)),2)*pow(sd_n,2)
    + pow(h1->GetEntries()/(2*I*I),2)*pow(sd_I,2));

  //Printing Results
  cout << "Measured Radius: " << r_m << " +- " << "0.001" << endl;
  cout << "Fitted Radius: " << r_f << " +- " << sd_r_f << endl;
  cout << "Calculated Radius: " << r_c << " +- " << sd_r_c << endl;

  //Saving Plots
  c1->Print("Fit-2.pdf");
  c2->Print("Hist-2.pdf");
}