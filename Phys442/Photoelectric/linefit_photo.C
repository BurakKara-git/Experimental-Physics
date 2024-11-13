{
  TTree *t = new TTree("t", "t");
  string color = "violet";
  t->ReadFile("violet.csv");		  
  float a,v;
  t->SetBranchAddress("A", &a);
  t->SetBranchAddress("V", &v);
  
  float * x, * y, * sx, * sy ;
  int n = t->GetEntries();
  x = new float[n];
  y = new float[n];
  sx = new float[n];
  sy = new float[n];

  float del_x = 0.1 * pow(10,-3);
  float del_y = 0.1 * pow(10,-14);
  
  float freqs[5] = {5.19, 5.49, 6.08,  6.88,  7.41}; // yellow, green, turq, blue, violet
  float freq = freqs[4] * pow(10,14);

  for (int i = 0; t->LoadTree(i) >= 0; i++){
  	t->GetEntry(i);
  	x[i] = v * pow(10,-3);
  	y[i] = a * pow(10,-14);
  }

  for (int i=0; i<n; ++i){
    sx[i] = del_x;
    sy[i] = del_y;
  }
  
  TGraphErrors *graph = new TGraphErrors(n,x,y,sx,sy);
  graph->Draw("A*");
  graph->SetTitle("Volt vs Amp for Violet");
  graph->GetXaxis()->SetTitle("Voltage(V)");
  graph->GetYaxis()->SetTitle("Current(A)");
  
  TF1 *line1 = new TF1("line", "[0] + [1]*x", 0, 0.8);
  line1->SetParameters(0,0);
  graph->Fit(line1,"R");
    
  TF1 *line2 = new TF1("line", "[0] + [1]*x", 1, 100);
  line2->SetParameters(0,0);
  graph->Fit(line2,"R+");  
  
  float m1 = line1->GetParameter(1);
  float m1_error = line1->GetParError(1);
  float n1 = line1->GetParameter(0);
  float n1_error = line1->GetParError(0);
  float m2 = line2->GetParameter(1);
  float m2_error = line2->GetParError(1);
  float n2 = line2->GetParameter(0);
  float n2_error = line2->GetParError(0);
  
  line2->Draw("same");
  //Error Propagation for the slope
  float up = n2-n1;
  float down = m1-m2;
  float up_error = sqrt(n2_error*n2_error + n1_error*n1_error);
  float down_error = sqrt(m1_error*m1_error + m2_error*m2_error);
  float x_intercept = (n2 - n1) / (m1 - m2);
  float error = x_intercept * sqrt((up_error/up)*(up_error/up)+(down_error/down)*(down_error/down));
  cout << "Stopping Voltage For " << color << " = " << x_intercept << "+-" << error << endl;
}
