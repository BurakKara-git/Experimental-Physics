 {
  TTree *tree = new TTree("tree", "tree");
  tree->ReadFile("length.csv");		  
  float c;
  tree->SetBranchAddress("c", &c);
  
  //Initializing Arrays
  float *L;
  int n = tree->GetEntries();
  L = new float[n];

	//Filling Arrays
  for (int i = 0; tree->LoadTree(i) >= 0; i++){
  	tree->GetEntry(i);
  	L[i] = c;
  }
	
	//Calculating alpha value
	float time = 0;	
	for (int i=0; i<n; i++){
	time += L[i];
	}	
	float alpha = (n+1) / time;
	//Filling Historam n=0 case
	TH1F *h1 = new TH1F("h1","n=0",60, 0.1, 60);  
    for (int i=0; i<n; i++) h1->Fill(L[i]); 
	//Filling Histogram n=1 case
	TH1F *h2 = new TH1F("h2","n=1",60, 0.1, 60);  
    for (int i=0; i<n-1; i++) h2->Fill(L[i]+L[i+1]);
  
  //Initializing Modified Poisson Functions  
  TF1 *f1 = new TF1("f1", "[0]*[1]*exp(-[1]*x)", 0.1, 60); //n=0
  TF1 *f2 = new TF1("f2", "[0]*[1]*[1]*x*exp(-[1]*x)", 0.1,60);
  f1->SetParNames("Poisson_Scale","Poisson_Alpha");
  f2->SetParNames("Poisson_Scale","Poisson_Alpha");

  gStyle->SetOptFit(1111);
	cout << alpha << endl;
	
	//Fitting Functions
  TCanvas *c1 = new TCanvas();
  f1->SetParameters(1, alpha);  
  h1->Fit("f1","R");
  h1->Draw(); 
  
  TCanvas *c2 = new TCanvas();
  f2->SetParameters(1, alpha);
  h2->Fit("f2","R");
  h2->Draw();
  
  //Creating Files
	c1->Print("length_0.pdf");
	c2->Print("length_1.pdf");

}
