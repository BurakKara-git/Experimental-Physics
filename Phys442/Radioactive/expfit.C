{
  TTree *t = new TTree("t", "t");
  TString file("4000-5.csv");
  t->ReadFile(file);
  TString title(file(0,4) + " Volt and " + file(5) + " Squeezes");	  
  float T;
  t->SetBranchAddress("T", &T);  
  float * times, * x, * y, * sx, * sy ;
  int n = t->GetEntries();
  times = new float[n];
  x= new float[n-1];
  y = new float[n-1];
  sx = new float[n-1];
  sy = new float[n-1];  

  float error = 0.300; //reflex error
  
  for (int i = 0; n > i; i++){
  	t->GetEntry(i);
	times[i] = T;
  }
  for (int i = 0; n-1 > i; i++){
  	y[i] = times[i+1] - times[i];
  	sy[i] = 1/y[i]*sqrt((error/y[i])*(error/y[i]));
  	y[i] = 1/y[i];
  	x[i] = (times[i+1] + times[i])/2;
  	sx[i] = error*sqrt(1/2);
  }
  TCanvas *c1 = new TCanvas();
  
  TGraphErrors *mygraph = new TGraphErrors(n,x,y,sx,sy);  
  mygraph->Draw("A*");
  mygraph->SetTitle(title);
  mygraph->GetXaxis()->SetTitle("Average Time(s)");
  mygraph->GetYaxis()->SetTitle("Charging Frequency(1/s)");
  TF1 *expo_fit = new TF1("expo_fit","[0]*exp([1]*x)",0.3,400);
  
  float par_0 = -0.001;
  float par_1 = -TMath::Log(2)/55.6;
  expo_fit->SetParameters(par_0,par_1);
  expo_fit->SetLineColor(kRed);
  expo_fit->SetLineWidth(2);
  mygraph->Fit(expo_fit,"R");
  mygraph->GetXaxis()->SetLimits(0, 400);
  mygraph->SetMinimum(0);
  mygraph->SetMaximum(5);
  expo_fit->Draw("same");
  gStyle->SetOptFit(1111);
  
  float decay = expo_fit->GetParameter(1);
  float decay_error = expo_fit->GetParError(1); 
  cout << "Decay Constant = " << -decay << "," << decay_error << endl;
  c1->Print(file(0,6) + ".pdf");
}
