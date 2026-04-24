static Double_t chi(double res);
static Double_t resEventPlane(double chi);

void Efficiency_27(int cen=5) {//0 means TPC, 1 means EPD outer, 11 means EPD inner


double Xfitting[9] = {4,4,4,4,4,4,4,4,4};
int left[9] = {70,60,50,40,30,20,10,5,0};
int right[9] = {80,70,60,50,40,30,20,10,5};
char centrality[200];
sprintf(centrality,"%d - %d %% 27 GeV Au+Au",left[cen-1],right[cen-1]);

char particle[200];
sprintf(particle,"p");//#pi^{+}


double YRange_fit[9] = {1,1,1,1,1,1,1,1,1}; 
double Xrange = 4;
double YRange = 1;

char printname[200];
sprintf(printname,"cen%d.eff.proton.pdf",cen);


char fname[200];
sprintf(fname,"cen%d.Proton.root",cen);

TFile *f = new TFile(fname);
TProfile * hSelPt = (TProfile*)f->Get("hSelPt");
TProfile * hSelPtMc = (TProfile*)f->Get("hSelPtMc");



int Num = 200;
float pT[Num], eff[Num], eff_err[Num];

for(int i =0;i<Num;i++){
	pT[i] = hSelPt->GetBinCenter(i);

	float temp1 = hSelPt->GetBinContent(i);
	float temp1_err = hSelPt->GetBinError(i);

	float temp2 = hSelPtMc->GetBinContent(i);
	float temp2_err = hSelPtMc->GetBinError(i);

	if(temp2!=0){
		eff[i] = temp1/temp2;
		eff_err[i] = eff[i]* sqrt(temp1_err* temp1_err/temp1/temp1 + temp2_err* temp2_err/temp2/temp2);
		}

	}
    gStyle->SetPalette(1);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetOptTitle(0);
    gStyle->SetOptDate(0);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetPadColor(0);
    gStyle->SetPadBorderSize(0);
    gStyle->SetCanvasBorderSize(0);
    gStyle->SetPadGridX( false );
    gStyle->SetPadGridY( false );
    gStyle->SetLabelSize(.05,"X");
    gStyle->SetLabelSize(.05,"Y");
    gStyle->SetTitleSize(.06,"X");
    gStyle->SetTitleSize(.06,"Y");

  gStyle->SetPadBorderMode (0);
  gStyle->SetPadColor (kWhite);
  gStyle->SetCanvasColor (kWhite);
  gStyle->SetFrameBorderMode (0);
  gStyle->SetCanvasBorderMode (0);
gStyle->SetTitleBorderSize (0);

TCanvas* can3 = new TCanvas("Flow_v", "Flow_v", 40,40,740,500);
 can3-> SetTopMargin(0.06);
 can3-> SetRightMargin(0.1);
 can3-> SetRightMargin(0.1);
 can3-> SetBottomMargin(0.17);
 can3->Draw();

  gPad->SetGridx(0);
  gPad->SetGridy(0);
   gPad->SetTickx();
   gPad->SetTicky();
  TString* histGraphName = new TString("Fl");
  TH1F* histGraph = new TH1F(histGraphName->Data(), "", 20, 0, Xrange);
  histGraph->SetMaximum(YRange);
  histGraph->SetMinimum(0);
  histGraph->SetLineColor(kBlack);
  histGraph->GetYaxis()->SetTitleOffset(0.67);
  histGraph->GetYaxis()->SetTitleSize(0.065);
  histGraph->GetXaxis()->SetTitleSize(0.08);
  histGraph->GetXaxis()->SetTitleOffset(0.90);
  histGraph->GetYaxis()->CenterTitle();
  histGraph->GetXaxis()->SetTitle("p_{T}");
  histGraph->GetXaxis()->CenterTitle();
  histGraph->GetYaxis()->SetTitle("Ratio");
  histGraph->GetXaxis()->SetNdivisions(6);
  histGraph->GetYaxis()->SetNdivisions(605);
  double lsize=histGraph->GetLabelSize();
  histGraph->GetYaxis()->SetLabelSize(lsize*1.0);
  histGraph->GetXaxis()->SetLabelSize(lsize*1.0);
  histGraph->Draw();

  TGraphErrors* eff_pT = new TGraphErrors(Num,pT,eff,0,eff_err);
  eff_pT->SetMarkerStyle(kOpenStar);
  eff_pT->SetMarkerSize(1.5);
  eff_pT->SetMarkerColor(2);
  eff_pT->SetLineColor(2);
  eff_pT->SetFillColor(4);
  eff_pT->SetLineStyle(1);
  eff_pT->SetLineWidth(2);
  eff_pT->Draw("pe1");

TF1 *fit_eff_pT = new TF1("fit_eff_pT","([0]+[3]*x+[4]*x*x)*exp(-pow([1]/x,[2]))",0,4);
fit_eff_pT->SetParameters(0.8,0.1,7);
fit_eff_pT->SetLineStyle(4);
fit_eff_pT->SetLineColor(4);
fit_eff_pT->SetLineWidth(4);

eff_pT->Fit("fit_eff_pT","E","",0,Xfitting[cen-1]);

float par1_sig = fit_eff_pT->GetParameter(0);
float par1_sig_err = fit_eff_pT->GetParError(0);
float par2_sig = fit_eff_pT->GetParameter(1);
float par2_sig_err = fit_eff_pT->GetParError(1);
float par3_sig = fit_eff_pT->GetParameter(2);
float par3_sig_err = fit_eff_pT->GetParError(2);
float par4_sig = fit_eff_pT->GetParameter(3);
float par4_sig_err = fit_eff_pT->GetParError(3);
float par5_sig = fit_eff_pT->GetParameter(4);
float par5_sig_err = fit_eff_pT->GetParError(4);

char function[200];
sprintf(function,"R = (%.2f #pm %.2f +  (%.2f #pm %.2f) p_{T} +  (%.2f #pm %.2f) p_{T}^{2}) #times", par1_sig, par1_sig_err, par4_sig, par4_sig_err,par5_sig, par5_sig_err);
cout <<function<<endl;

char function2[200];
sprintf(function2,"exp ( (- #frac{%.3f #pm %.3f}{p_{T}} )^{%.2f #pm %.2f})",par2_sig, par2_sig_err, par3_sig, par3_sig_err);


   TLatex *   tex = new TLatex(Xrange*0.2, YRange*0.32, centrality);
   tex->SetTextSize(0.08);
   tex->SetTextColor(1);
   tex->Draw();

   TLatex *   tex2 = new TLatex(Xrange*0.65, YRange*0.08, particle);
   tex2->SetTextSize(0.08);
   tex2->SetTextColor(1);
   tex2->Draw();

   TLatex *   tex3 = new TLatex(Xrange*0.15, YRange*0.22, function);
   tex3->SetTextSize(0.037);
   tex3->SetTextColor(4);
   tex3->Draw();
   TLatex *   tex4 = new TLatex(Xrange*0.25, YRange*0.15, function2);
   tex4->SetTextSize(0.037);
   tex4->SetTextColor(4);
   tex4->Draw();


TLegend* legend = new TLegend(0.7, 0.2, 0.8, 0.3);
 legend->SetFillColor(0);
 legend->SetTextSize(0.065);
 legend->SetLineColor(0);
 legend->SetBorderSize(0);
 legend->SetLineStyle(3);
 legend->AddEntry(eff_pT, "Ratio","p");
 legend->Draw();


can3->Print(printname);

}