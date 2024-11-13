// Line fitting macro written for ROOT
{
  const int ndata = 11;

  float x[ndata] = {300,320,340,360,  380,  400,420,440,460,480,500};
  float y[ndata] = {0,0,0,1223,1236,1335,1387,1352,1299,1317,1289};
  
  TGraphErrors *mygraph = new TGraphErrors(ndata,x,y,0,0);
  mygraph->Draw("A*");
  TF1 *fnew = new TF1("fnew","[0]*x+[1]",360,500);
  fnew->SetParameters(0.01,1300);
  mygraph->Fit(fnew,"R");  
}
