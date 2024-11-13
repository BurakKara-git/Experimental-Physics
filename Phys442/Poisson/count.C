 {
  TTree *tree = new TTree("tree", "tree");
  tree->ReadFile("count.csv");		  
  float cs10,cs1,br1,br10;
  tree->SetBranchAddress("cs10", &cs10);
  tree->SetBranchAddress("cs1", &cs1);
  tree->SetBranchAddress("br1", &br1);
  tree->SetBranchAddress("br10", &br10);
  
  //Initializing Arrays
  float *c1_10, *c1_1, *c2_1, *c2_10;
  int n = tree->GetEntries();
  c1_10 = new float[n];
  c1_1 = new float[n];
  c2_1 = new float[n];
  c2_10 = new float[n]; 

	//Filling Arrays
  for (int i = 0; tree->LoadTree(i) >= 0; i++){
  	tree->GetEntry(i);
  	c1_10[i] = cs10;
  	c1_1[i] = cs1;
  	c2_10[i] = br10;
  	c2_1[i] = br1;
  }
	
	//Filling Histograms
  TH1F *h1 = new TH1F("h1","Caesium-137, 10 Seconds - Gauss Fit",100, 0.0, 200);  
    for (int i=0; i<n; i++) h1->Fill(c1_10[i]); 
  h1->GetXaxis()->SetTitle("Count");
  h1->GetYaxis()->SetTitle("Occurrence"); 
  TH1F *h2 = new TH1F("h2","Caesium-137, 1 Second - Gauss Fit",10, 0.0, 20);  
    for (int i=0; i<n; i++) h2->Fill(c1_1[i]);
  h2->GetXaxis()->SetTitle("Count");
  h2->GetYaxis()->SetTitle("Occurrence");     
  TH1F *h3 = new TH1F("h3","Barium-133, 10 Seconds - Gauss Fit",25, 0.0, 50);  
    for (int i=0; i<n; i++) h3->Fill(c2_10[i]);
  h3->GetXaxis()->SetTitle("Count");
  h3->GetYaxis()->SetTitle("Occurrence");     
  TH1F *h4 = new TH1F("h4","Barium-133, 1 Second - Gauss Fit",10, 0.0, 10);  
    for (int i=0; i<n; i++) h4->Fill(c2_1[i]);
  h4->GetXaxis()->SetTitle("Count");
  h4->GetYaxis()->SetTitle("Occurrence");
       
  //Creating Clones For Poisson Fit
  TH1F* h1c = (TH1F*)h1->Clone();
  h1c->SetTitle("Caesium-137, 10 Seconds - Poisson Fit");
  TH1F* h2c = (TH1F*)h2->Clone();
  h2c->SetTitle("Caesium-137, 1 Second - Poisson Fit");
  TH1F* h3c = (TH1F*)h3->Clone();
  h3c->SetTitle("Barium-133, 10 Seconds - Poisson Fit");
  TH1F* h4c = (TH1F*)h4->Clone();
  h4c->SetTitle("Barium-133, 1 Second - Poisson Fit");
  
  //Initializing Gaus and Poisson Functions
  TF1 *f1 = new TF1("f1","gaus",0,200);
  TF1 *f2 = new TF1("f2", "[0]*TMath::Poisson(x,[1])",0,200);
  f1->SetParNames("Gauss_Constant","Gauss_Mean","Gauss_Sigma");
  f2->SetParNames("Poisson_Scale","Poisson_Mean");
  
  //Calculations for Linear Fit
  double x[4] = {h1->GetMean(), h2->GetMean(), h3->GetMean(), h4->GetMean()};
  double y[4] = {(sqrt(h1->GetMean())/h1->GetRMS()),
  (sqrt(h2->GetMean())/h2->GetRMS()),
  (sqrt(h3->GetMean())/h3->GetRMS()),
  (sqrt(h4->GetMean())/h4->GetRMS())};

  gStyle->SetOptFit(1111);

	//Fitting Functions
  TCanvas *c1 = new TCanvas(); // Linear Fit
  TGraph *mygraph = new TGraphErrors(n,x,y);
	mygraph->SetTitle(" ;Mean;Sqrt(Mean)/Standart Deviation");
	mygraph->Draw("A*");
	TF1 *fnew = new TF1("fnew","[0]*x+[1]",2,120);
	fnew->SetParameters(0,1);
	mygraph->Fit(fnew, "R");

	TCanvas *c2 = new TCanvas(); //Gauss Fit  
  f1->SetParameters(h1->GetMaximum(), h1->GetMean(), h1->GetRMS() );  
  h1->Fit("f1");
  h1->Draw(); 

	TCanvas *c3 = new TCanvas(); //Poisson Fit	
  f2->SetParameters(h1c->GetMaximum(), h1c->GetMean());
  h1c->Fit("f2", "R"); 
  h1c->Draw();  
  
  TCanvas *c4 = new TCanvas(); //Gauss Fit  
  f1->SetParameters(h2->GetMaximum(), h2->GetMean(), h2->GetRMS() );
  f1->SetRange(0.0,20);
  h2->Fit("f1");
  h2->Draw(); 

	TCanvas *c5 = new TCanvas(); //Poisson Fit	
  f2->SetParameters(h2c->GetMaximum(), h2c->GetMean());
  f2->SetRange(0.0,20);
  h2c->Fit("f2", "R"); 
  h2c->Draw();  
  
  TCanvas *c6 = new TCanvas(); //Gauss Fit  
  f1->SetParameters(h3->GetMaximum(), h3->GetMean(), h3->GetRMS() );
  f1->SetRange(0.0,50);  
  h3->Fit("f1");
  h3->Draw(); 

	TCanvas *c7 = new TCanvas(); //Poisson Fit	
  f2->SetParameters(h3c->GetMaximum(), h3c->GetMean());
  f2->SetRange(0.0,50);
  h3c->Fit("f2", "R"); 
  h3c->Draw();  
  
  TCanvas *c8 = new TCanvas(); //Gauss Fit  
  f1->SetParameters(h4->GetMaximum(), h4->GetMean(), h4->GetRMS() );
  f1->SetRange(0.0,10); 
  h4->Fit("f1");
  h4->Draw(); 

	TCanvas *c9 = new TCanvas(); //Poisson Fit	
  f2->SetParameters(h4c->GetMaximum(), h4c->GetMean());
  f2->SetRange(0.0,10);
  h4c->Fit("f2", "R"); 
  h4c->Draw();  
  
  //Creating Files
	c1->Print("linear.pdf");
	c2->Print("cs137-10-gauss.pdf");
	c3->Print("cs137-10-poiss.pdf");
	c4->Print("cs137-1-gauss.pdf");
	c5->Print("cs137-1-poiss.pdf");
	c6->Print("ba133-10-gauss.pdf");
	c7->Print("ba133-10-poiss.pdf");
	c8->Print("ba133-1-gauss.pdf");
	c9->Print("ba133-1-poiss.pdf");
}
