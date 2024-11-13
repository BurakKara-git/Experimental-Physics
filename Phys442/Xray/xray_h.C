{
  const int n = 6;
  double q = 1.602176634e-19;
  double freq[n] = {3.76064e+18,4.64811e+18,5.27529e+18,5.97956e+18,6.97476e+18,7.07684e+18};
  double sd_freq[n] = {6.40308e+17, 8.3406e+17 , 7.82979e+17, 9.95806e+17 ,6.10088e+17, 6.56757e+17};
  double energy[n] = {q*15e3, q*18e3, q*21e3, q*24e3, q*27e3, q*30e3};
  double sd_energy[n] = {q*1e3,q*1e3,q*1e3,q*1e3,q*1e3,q*1e3};
  
  TCanvas *c1 = new TCanvas();
  TGraphErrors *graph = new TGraphErrors(n,freq,energy,sd_freq,sd_energy);  
  TF1 *f_linear = new TF1("f_linear","[0] + [1]*x",1e18,7e18);
  f_linear->SetParameters(0,6e-34);
  f_linear->SetParNames("Interception","Planck");
  graph->Fit("f_linear","R");
  graph->SetTitle("Energy vs Frequency; Frequency(Hertz);  Energy(J)");
  graph->SetMarkerStyle(20);
  graph->Draw("AP");
  gStyle->SetOptFit(1111);

  double planck = f_linear->GetParameter(1);
  double sd_planck = f_linear->GetParError(1);
  double h = 6.62607015e-34;

  cout << "Experimental Planck Constant: " << planck << " +- " << sd_planck << endl;
  cout << "Relative Error: " << abs(planck-h)*100/h << "%"<< endl;
  cout << abs(planck-h)/sd_planck << " Sigma Away" << endl;
  c1->Print("xray_h.pdf");
  }