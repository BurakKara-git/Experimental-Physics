{
  const int ndata = 5;
  double y[ndata] = { 0.786,  0.407,  0.347,  0.404,  0.288};
  double x[ndata] = {5.19,  5.49, 6.08, 6.88, 7.41};
  double sy[ndata] = {0.0026358,  0.0122895,  0.00611123,  0.0034491,  0.00693817};
  double sx[ndata] = {0,0,0,0,0};
  
  double lambda_error = 10.0 * pow(10,-9);
  double light = 3.0*pow(10,8);
  
  for (int i = 0; i<ndata; i++){ // Error Propogation for Frequency
  double lambda = light/x[i];
  x[i] *= pow(10,14);
  sx[i] = x[i] * (lambda_error/lambda); // Error of Speed of Light = 0
  
  }
  double xmin = 300*pow(10,12);
  TGraphErrors *mygraph = new TGraphErrors(ndata,x,y,sx,sy);
  mygraph->Draw("A*");
  
  TF1 *fnew = new TF1("fnew","[0]*x+[1]",0,1*pow(10,15));
  fnew->SetParameters(0,0.5); // arbitrary starting parameters
  
  
  mygraph->Fit("fnew","M");
  
  mygraph->SetTitle("Stopping Voltage vs Frequency");
  mygraph->GetYaxis()->SetTitle("Voltage(V)");
  mygraph->GetXaxis()->SetTitle("Frequenct(Hz)");
  double slope = fnew->GetParameter(0);
  double y_int = fnew->GetParameter(1);
  double s_error = fnew->GetParError(0);
  double y_error = fnew->GetParError(1);
  cout << "h/q Ratio = "<< slope << " +- " << s_error << endl;
  cout << "Intercept of the Line = " << y_int << " +- " << y_error << endl;
}
