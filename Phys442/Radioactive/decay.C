 {
  TTree *tree = new TTree("tree", "tree");
  tree->ReadFile("decay_data.csv");		  
  float t,st,v,s;
  tree->SetBranchAddress("t", &t);
  tree->SetBranchAddress("st", &st);
  tree->SetBranchAddress("v", &v);
  tree->SetBranchAddress("s", &s);
  
  float * decay, * decay_error, * volt, * sq, * volt_error;
  int n = tree->GetEntries();
  decay = new float[n];
  decay_error = new float[n];
  volt = new float[n];
  squ = new float[n]; 
  volt_error = new float[n];


  for (int i = 0; tree->LoadTree(i) >= 0; i++){
  	tree->GetEntry(i);
  	decay[i] = t;
  	decay_error[i] = st;
  	volt[i] = v;
  	squ[i] = s;
  	volt_error[i] = 100;
  }
  TCanvas *c1 = new TCanvas(); 
  TGraphErrors *mygraph = new TGraphErrors(n,volt,decay,volt_error,decay_error);
  mygraph->Draw("A*");
  mygraph->SetTitle("Decay vs Volt");
  mygraph->GetYaxis()->SetTitle("Decay Constant");
  mygraph->GetXaxis()->SetTitle("Volt(V)");

  TCanvas *c2 = new TCanvas();
  TGraphErrors *mygraph2 = new TGraphErrors(n,squ,decay,0,decay_error);
  mygraph2->Draw("A*");
  mygraph2->SetTitle("Decay vs Squeezes");
  mygraph2->GetYaxis()->SetTitle("Decay Constant");
  mygraph2->GetXaxis()->SetTitle("Squeezes");

  float weight = 0;
  float totw = 0;
  float decaybar = 0;
  float decay_errorbar = 0;
  
  for (int i=0; i<n; ++i) {
    weight = 1./(decay_error[i]*decay_error[i]);
    totw += weight;
    decaybar += (decay[i]*weight);
    //life_errorbar += (life_error[i]*weight);
  }

  decaybar /= totw;
  decay_errorbar = sqrt(1 / totw);

  float life = TMath::Log(2)/decaybar;
  float life_error = life*sqrt((decay_errorbar/decaybar)*(decay_errorbar/decaybar));
  
  cout << "Weighted Average of Decay Constant = " << decaybar << " +- " << decay_errorbar << endl;
  cout << "Weighted Average of Half-Life = " << life << " +- " << life_error << endl;
   }
