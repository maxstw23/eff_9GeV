#ifndef __CINT__
#include "TROOT.h"
#include "TSystem.h"
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TTreeHelper.h"
#include "TDatime.h"
#include "StarRoot/TUnixTime.h"
#include "StChain.h"
#include "StMessMgr.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuMcTrack.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StBTofHeader.h"
#include "StThreeVectorF.hh"
#include "StPhysicalHelixD.hh"
#endif

void Divide(){
TH1F *hSelPt = new TH1F("hSelPt","Pt of selected matched RC tracks",38,0.2,4.0);
TH1F *hSelPt2 = new TH1F("hSelPt2","Pt of selected matched RC tracks",38,0.2,4.0);
  hSelPt->Sumw2();
hSelPt2->Sumw2();
for(int i =0; i<100;i++){
hSelPt2->Fill(i*i/2500.);
hSelPt->Fill(i*i/2500.);
}
hSelPt2->Draw();
hSelPt->Divide(hSelPt2);
hSelPt->Draw("same");
hSelPt2->SetLineColor(kRed);
//hSelPt->Draw("same");
}

