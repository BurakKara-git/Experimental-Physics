 {
  TTree *tree = new TTree("tree", "tree");
  tree->ReadFile("XRAY_data/30.csv");
  double volt = 30*1e3;
  double x_min = 0.045e-9;
  double x_max = 0.06e-9;
  float a, I;
  tree->SetBranchAddress("a", &a);
  tree->SetBranchAddress("I", &I);

  //Initializing Arrays
  double *count, *angle, *sd_count, *sd_angle;
  double *wave, *sd_wave, *extend;
  int n = tree->GetEntries();
  wave = new double[n];
  count = new double[n];
  angle = new double[n];
  sd_count = new double[n];
  sd_angle = new double[n];
  sd_wave = new double[n];
  extend = new double[n];

  //Inıtializing Constants
  double h = 6.62607015e-34;
  double light = 299792458.;
  double eV = 1.60217663e-19;
  double K_a1 = 17479.34*eV;
  double K_a2 = 17374.3*eV;
  double K_a = (2*K_a1 + 1*K_a2)/3;
  double K_b = 19608.3*eV;
  double deg2rad = TMath::Pi()/180;
  double slope = 0.92199;
  double sd_slope = 0.001688;
  double intercept = 0.15944;
  double sd_intercept = 0.0308526;
  double d = 282.0e-12;
  double sd_d = 0.1e-12;

  //Filling Arrays
  for (int i = 0; tree->LoadTree(i) >= 0; i++){
  	tree->GetEntry(i);
  	count[i] = I;
    angle[i] = a*slope + intercept;
    wave[i] = 2*d*TMath::Sin(deg2rad*angle[i]);
    sd_count[i] = sqrt(count[i]/5);
    sd_angle[i] = sqrt(pow(slope,2)*pow(0.1,2) + 
    pow(a,2)*pow(sd_slope,2) + 
    pow(1,2)*pow(sd_intercept,2));
    sd_wave[i] = sqrt(pow(2*TMath::Sin(deg2rad*angle[i]),2)*pow(sd_d,2) +
    pow(2*d*TMath::Cos(deg2rad*angle[i])*deg2rad,2)*pow(sd_angle[i],2));
  }
	

  //Linear Fit
  gStyle->SetOptFit(1111);
  TCanvas *c1 = new TCanvas();
  TGraphErrors *graph = new TGraphErrors(n,wave,count,sd_wave,sd_count);  
  TF1 *f_linear = new TF1("f_linear","[0] + [1]*x",x_min,x_max);
  double par_1 = 1e12;
  double par_0 = -x_min * par_1;
  f_linear->SetParameters(par_0,par_1);
  f_linear->SetParNames("Interception","Slope");
  graph->Fit("f_linear","R");
  double new_slope = f_linear->GetParameter(1);
  double new_intercept = f_linear->GetParameter(0);
  double sd_new_slope = f_linear->GetParError(1);
  double sd_new_intercept = f_linear->GetParError(0);
  for(int i = 0; i < n; i++){
    extend[i] = new_slope*wave[i] + new_intercept;
  }
  TGraph *graph2 = new TGraph(n,wave,extend);
  graph->SetTitle("Accumulation Rate vs Wavelenght; Wavelenght(m);  Accumulation Rate (Impulse/Sec)");
  graph->SetMarkerStyle(20);
  graph->Draw("AP");
  graph2->SetLineColor(2);

  graph2->Draw("Same");
  

  //Calculating Lambda_Min and Freq_Max
  double lambda = -new_intercept/new_slope;
  double sd_lambda = sqrt(pow(-1/new_slope,2)*pow(sd_new_intercept,2) + 
  pow(new_intercept/pow(new_slope,2),2)*pow(sd_new_slope,2));
  double freq = light/lambda;
  double sd_freq = sqrt(pow(light/(lambda*lambda),2)*pow(sd_lambda,2));
  cout << "Lambda_Min: " << lambda << " +- " << sd_lambda << endl;
  cout << "Freq_Max: " << freq << " +- " << sd_freq << endl;
  cout << "Volt: " << volt << " +- " << 1e3 << endl;
  c1->Print("30.pdf");
}