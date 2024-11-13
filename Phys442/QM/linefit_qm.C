// Line fitting macro written for ROOT
{
  TTree *t = new TTree("t", "t");
  t->ReadFile("qm_all.csv");		  
  float a,v,r;
  t->SetBranchAddress("a", &a);
  t->SetBranchAddress("v", &v);
  t->SetBranchAddress("r", &r);
  
  float * amp, * volt, * rad, * x, * y, * sx, * sy ;
  int n = t->GetEntries();
  amp = new float[n];
  volt = new float[n];
  rad = new float[n];
  x= new float[n];
  y = new float[n];
  sx = new float[n];
  sy = new float[n];  


  for (int i = 0; t->LoadTree(i) >= 0; i++){
  	t->GetEntry(i);
  	amp[i] = a;
  	volt[i] = v;
  	rad[i] = r;
  }

  float del_r = 0.002;
  float del_v = 1;
  float del_a = 0.01;
  
  float cons = 2086069.919;
   
  for (int i=0; i<n; ++i){ // Error Propagation
    float r2 = rad[i]*rad[i];
    float a2 = amp[i]*amp[i];
    float del_r2 = r2 * 2 * del_r / rad[i];
    
    x[i] = r2 / (2 * volt[i]);
    y[i] = cons / a2;    
    sx[i] = x[i] * sqrt( (del_v/volt[i]) * (del_v/volt[i]) + (del_r2 / r2) * (del_r2 / r2));
    sy[i] = (cons/a2) * 2 * del_a / amp[i];
  }
  
  float weight = 0;
  float totw = 0; // Total weight
  float xybar = 0, xbar = 0, ybar = 0, x2bar = 0, y2bar = 0; // weighted averages

  for (int i=0; i<n; ++i) {
    weight = 1./((sy[i]*sy[i]) + (sx[i]*sx[i]));
    totw += weight;
    xybar += (x[i]*y[i]*weight);
    xbar += (x[i]*weight);
    ybar += (y[i]*weight);
    x2bar += (x[i]*x[i]*weight);
    y2bar += (y[i]*y[i]*weight);
  }

  float sy2bar = n; // weighted average error squared
  float slope = (xybar - xbar*ybar) / (x2bar - xbar*xbar);
  float itcpt = ybar - slope * xbar;
  float slopeerr = sqrt ( sy2bar / (n * (x2bar - xbar*xbar) ) );
  float itcpterr = sqrt ( x2bar ) * slopeerr;
  cout << slopeerr << endl;
  cout << "slope of fit line = " << slope << " +- " << slopeerr << endl;
  cout << "intercept of fit line = " << itcpt << " +- " << itcpterr << endl;

  TGraphErrors *mygraph = new TGraphErrors(n,x,y,sx,sy);
  mygraph->Draw("A*");

  gStyle->SetOptFit(1111);
  
  TF1 *ffitline = new TF1("ffitline","[0]*x+[1]",0,6);
  ffitline->SetParameter(0,slope);
  ffitline->SetParameter(1,itcpt);
  ffitline->SetLineColor(kBlue); // draw in blue color
  ffitline->SetLineStyle(2); // draw dotted line
  ffitline->Draw("same");

  // let's also try the same with ROOT's own fitter
  TF1 *fnew = new TF1("fnew","[0]*x+[1]",0,6);
  fnew->SetParameters(600,7); // arbitrary starting parameters
  mygraph->Fit(fnew);
}
