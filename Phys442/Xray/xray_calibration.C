 {
  TTree *tree = new TTree("tree", "tree");
  tree->ReadFile("XRAY_data/calibration.csv");		  
  float c, a;
  tree->SetBranchAddress("c", &c);
  tree->SetBranchAddress("a", &a);

  //Initializing Arrays
  double *count, *angle, *sd_count, *sd_angle;
  double *experimental, *sd_experimental;
  double *theoretical;
  int n_angle = 6;
  int n = tree->GetEntries();
  count = new double[n];
  angle = new double[n];
  sd_count = new double[n];
  sd_angle = new double[n];
  theoretical = new double[n_angle];
  experimental = new double[n_angle];
  sd_experimental = new double[n_angle];

  //Filling Arrays
  for (int i = 0; tree->LoadTree(i) >= 0; i++){
  	tree->GetEntry(i);
  	count[i] = c;
    angle[i] = a;
    sd_count[i] = sqrt(count[i]/5);
    sd_angle[i] = 0.1;
  }
	
  //Inıtializing Constants
  double h = 6.62607015e-34;
  double light = 299792458.;
  double d = 282.0e-12;
  double sd_d = 0.1e-12;
  double eV = 1.60217663e-19;
  double K_a1 = 17479.34*eV;
  double K_a2 = 17374.3*eV;
  double K_a = (2*K_a1 + 1*K_a2)/3;
  double K_b = 19608.3*eV;
  double rad2deg = 180/TMath::Pi();

  //Calculating Theoretical Values
  for(int m = 0; m < 4; m++){

  double theta_a = rad2deg*TMath::ASin((m+1)*h*light/(2*K_a*d));
  double theta_b = rad2deg*TMath::ASin((m+1)*h*light/(2*K_b*d));

  if(m == 0){
    theoretical[m] = (theta_a+theta_b)/2;
  }
  else{
    if(m == 3){
      theoretical[2*m-1] = theta_b;
    }
    else{
    theoretical[2*m - 1] = theta_b;
    theoretical[2*m] = theta_a;
    }
  }
  cout << "Theoretical K alpha for m = " << m+1 << ": "<< theta_a << endl;
  cout << "Theoretical K beta for m = " << m+1 << ": "<< theta_b << endl;
  }

  //Gauss Fit
  TCanvas *c1 = new TCanvas();
  TGraphErrors *graph = new TGraphErrors(n,angle,count,sd_angle,sd_count);
  graph->SetTitle("Count vs Angle; Angle(Deg);Count");
  

  TF1 *f1 = new TF1("f1","gaus",7.2,8);
  TF1 *f2 = new TF1("f2","gaus",13.2,13.7);
  TF1 *f3 = new TF1("f3","gaus",14.8,15.3);
  TF1 *f4 = new TF1("f4","gaus",19.5,20.5);
  TF1 *f5 = new TF1("f5","gaus",22.2,23);
  TF1 *f6 = new TF1("f6","gaus",30.2,31);

  graph->Fit(f1,"R");  
  graph->Fit(f2,"R+");
  graph->Fit(f3,"R+");
  graph->Fit(f4,"R+");  
  graph->Fit(f5,"R+");
  graph->Fit(f6,"R+");

  //Filling Experimental Values
  experimental[0] = f1->GetParameter(1);
  sd_experimental[0] = f1->GetParError(1);

  experimental[1] = f2->GetParameter(1);
  sd_experimental[1] = f2->GetParError(1);  

  experimental[2] = f3->GetParameter(1);
  sd_experimental[2] = f3->GetParError(1);

  experimental[3] = f4->GetParameter(1);
  sd_experimental[3] = f4->GetParError(1);

  experimental[4] = f5->GetParameter(1);
  sd_experimental[4] = f5->GetParError(1);

  experimental[5] = f6->GetParameter(1);
  sd_experimental[5] = f6->GetParError(1);

  graph->SetMarkerStyle(20);
  graph->Draw("AP");
  //Linear Fit for Calibration
  TCanvas *c2 = new TCanvas();
  TGraphErrors *graph_c = new TGraphErrors(n_angle,experimental,theoretical,sd_experimental,0);
  TF1 *f_linear = new TF1("f_linear","[0] + [1]*x",0,35);
  f_linear->SetParameters(0,1);
  f_linear->SetParNames("Interception","Slope");
  graph_c->Fit("f_linear","RE"); //No error on x
  graph_c->SetTitle("Theoretical vs Experimental Angle;Experimentel Angle(Deg);Theoretical Angle(Deg)");
  graph_c->SetMarkerStyle(20);
  graph_c->Draw("AP");
  gStyle->SetOptFit(1111);

  double slope = f_linear->GetParameter(1);
  double intercept = f_linear->GetParameter(0);
  double sd_slope = f_linear->GetParError(1);
  double sd_intercept = f_linear->GetParError(0);
  cout << "Slope of the calibration = " << slope << " +- " << sd_slope << endl;
  cout << "Intercept of the calibration = " << intercept << " +- " << sd_intercept << endl;

  for(int i = 0; i < n_angle; i++){
    cout << "Experimental Angle For Spike-" << i+1 << " " << experimental[i] << " +- " << sd_experimental[i] << endl;;
    cout << "Theoretical Angle For Spike-" << i+1 <<" " << theoretical[i] << endl;
  }
  //Printing Results
  c1->Print("xray_gauss.pdf");
	c2->Print("xray_calibration.pdf");
}