// Xianglei Zhu
// Skeleton embedding MuDST analysis macro with StMuDSTMaker
// Run it with the wrapper in ACLIC mode, CINT mode for debug ONLY

#ifndef __CINT__
#include "TROOT.h"
#include "TSystem.h"
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TProfile2D.h"
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
#include "/star/data01/pwg/xiatong/git/eff_9GeV/StRoot/StRefMultCorr/StRefMultCorr.h"
#include "/star/data01/pwg/xiatong/git/eff_9GeV/StRoot/StRefMultCorr/CentralityMaker.h"
#include "StBTofHeader.h"
#include "StThreeVectorF.hh"
#include "StPhysicalHelixD.hh"
#endif

const int cenDef[10] = {6, 13, 24, 43, 70, 110, 165, 243, 296, 500}; // 19.6 GeV 2011
// pid: 8 PI+, 9 PI-, 11 K+, 12 K-, 14 PROTON, 15 ANTIPROTON
const float eta_cut = 1.5;

void makeMuDstQA(TString InputFileList, Int_t cen = 5, Int_t nFiles = 1, Int_t nEvents = 0, TString OutputFile = "test.histo.root", Int_t pid = 8);

void makeMuDstQA(TString InputFileList, Int_t cen, Int_t nFiles, Int_t nEvents, TString OutputFile, Int_t pid)
{

   // Load libraries for CINT mode
#ifdef __CINT__
   gROOT->Macro("loadMuDst.C");
#endif

   // List of member links in the chain
   StChain *chain = new StChain;

   StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", InputFileList, "MuDst", nFiles);

   // ---------------- modify here according to your QA purpose --------------------------
   // Turn off everything but Primary tracks in order to speed up the analysis and eliminate IO
   muDstMaker->SetStatus("*", 0);               // Turn off all branches
   muDstMaker->SetStatus("MuEvent", 1);         // Turn on the Event data (esp. Event number)
   muDstMaker->SetStatus("PrimaryVertices", 1); // Turn on the primary track data
   muDstMaker->SetStatus("PrimaryTracks", 1);   // Turn on the primary track data
   muDstMaker->SetStatus("GlobalTracks", 1);    // Turn on the global track data
   muDstMaker->SetStatus("CovGlobTrack", 1);    // to fix the refmult in Run14!!!
   muDstMaker->SetStatus("MCAll", 1);           // Turn on the McVertex/McTrack data
   muDstMaker->SetStatus("BTofHeader", 1);      // Turn on the btof data
   muDstMaker->SetDebug(0);                     // Turn off Debug information

   if (nEvents == 0)
      nEvents = 10000000; // Take all events in nFiles if nEvents = 0

   // ---------------- modify here according to your QA purpose --------------------------
   TFile *tags_output = new TFile(OutputFile, "recreate");
   tags_output->cd();

   // book histograms or trees if you need
   TH1F *hPhiMc = new TH1F("hPhiMc", "Phi of Mc tracks", 200, -TMath::Pi(), TMath::Pi());
   TH1F *hPtMc = new TH1F("hPtMc", "Pt of Mc tracks", 200, 0, 5.0);
   TH1F *hSelPtMc = new TH1F("hSelPtMc", "Pt of selected Mc tracks", 100, 0, 4);
   TH1F *hSelPMc = new TH1F("hSelPMc", "P of selected Mc tracks", 100, 0, 4);
   TH2F *hSelPtEtaMc = new TH2F("hSelPtEtaMc", "Pt vs Eta of selected Mc tracks", 100, 0, 4, 200, -2.0, 2.0);
   TH1F *hEtaMc = new TH1F("hEtaMc", "Eta of Mc tracks", 200, -2.0, 2.0);
   TH1F *hPhi = new TH1F("hPhi", "Phi of matched RC tracks", 200, -TMath::Pi(), TMath::Pi());
   TH1F *hPt = new TH1F("hPt", "Pt of matched RC tracks", 200, 0, 5.0);
   TH1F *hSelPt = new TH1F("hSelPt", "Pt of selected matched RC tracks", 100, 0, 4);
   TH1F *hSelP = new TH1F("hSelP", "P of selected matched RC tracks", 100, 0, 4);
   TH2F *hSelPtEta = new TH2F("hSelPtEta", "Pt vs Eta of selected matched RC tracks", 100, 0, 4, 200, -2.0, 2.0);
   TProfile *hSelFirstPointDis = new TProfile("hSelFirstPoint", "FirstPoint of selected matched RC tracks", 100, 0, 4, 0, 200);
   TProfile *hSelLastPointDis = new TProfile("hSelLastPoint", "LastPoint of selected matched RC tracks", 100, 0, 4, 0, 200);
   TH1F *hEffPt = new TH1F("hEffPt", "Efficiency in pt bins", 200, 0., 4);
   TH1F *hEta = new TH1F("hEta", "Eta of matched RC tracks", 200, -2.0, 2.0);
   TH1F *histRefMult = new TH1F("histRefMult", "histRefMult of muDst tracks", 500, 0, 500);
   TH1F *histRefMult2 = new TH1F("histRefMult2", "histRefMult 2 of muDst tracks", 500, 0, 500);
   TH2D *hnSigmaPt = new TH2D("hnSigmaPt", "Pt versus hnSigma of RC tracks", 50, 0, 5, 200, -10, 10);

   TProfile *hEnergyLostPt = new TProfile("hEnergyLostPt", "hEnergyLostPt", 100, 0, 4, -10., 10.);
   TProfile2D *hEnergyLostPtEta = new TProfile2D("hEnergyLostPtEta", "hEnergyLostPtEta", 100, 0, 4, 100, -2.0, 2.0, -10., 10.);
   TProfile *hEnergyLostMCPt = new TProfile("hEnergyLostMCPt", "hEnergyLostMCPt", 100, 0, 4, -10., 10.);
   TProfile2D *hEnergyLostMCPtEta = new TProfile2D("hEnergyLostMCPtEta", "hEnergyLostMCPtEta", 100, 0, 4, 100, -2.0, 2.0, -10., 10.);
   TProfile *hSmearPx = new TProfile("hSmearPx", "hSmearPx", 100, 0, 4, -10., 10.);
   TProfile *hSmearPy = new TProfile("hSmearPy", "hSmearPy", 100, 0, 4, -10., 10.);
   TProfile *hSmearPz = new TProfile("hSmearPz", "hSmearPz", 100, 0, 4, -10., 10.);
   TProfile2D *hSmearPxEta = new TProfile2D("hSmearPxEta", "hSmearPxEta", 100, 0, 4, 100, -2.0, 2.0, -10., 10.);
   TProfile2D *hSmearPyEta = new TProfile2D("hSmearPyEta", "hSmearPyEta", 100, 0, 4, 100, -2.0, 2.0, -10., 10.);
   TProfile2D *hSmearPzEta = new TProfile2D("hSmearPzEta", "hSmearPzEta", 100, 0, 4, 100, -2.0, 2.0, -10., 10.);

   hEffPt->Sumw2();
   hSelPt->Sumw2();
   hSelPtEta->Sumw2();
   hSelPtMc->Sumw2();
   hSelPtEtaMc->Sumw2();
   hSelFirstPointDis->Sumw2();
   hSelLastPointDis->Sumw2();

   // ---------------- end of histogram and tree booking --------------------------------

   // Loop over the links in the chain
   Int_t iInit = chain->Init();
   if (iInit)
      chain->FatalErr(iInit, "on init");

   StRefMultCorr *refmultCorrUtil = CentralityMaker::instance()->getRefMultCorr();

   Int_t istat = 0, i = 1;
   while (i <= nEvents && istat != 2)
   {
      if (i % 10 == 0)
         cout << endl
              << "== Event " << i << " start ==" << endl;
      chain->Clear();
      istat = chain->Make(i);

      if (istat == 2)
         cout << "Last  event processed. Status = " << istat << endl;
      if (istat == 3)
         cout << "Error event processed. Status = " << istat << endl;
      i++;

      if (istat != kStOK)
         continue;

      StMuDst *mMuDst = muDstMaker->muDst();
      if (!mMuDst)
      {
         LOG_WARN << " No MuDst " << endm;
         continue;
      }

      StMuEvent *mMuEvent = mMuDst->event();
      if (!mMuEvent)
      {
         LOG_WARN << " No MuEvent " << endm;
         continue;
      }

      //-----------------------------------------------------------------------------
      // vertex selection
      int const originalVertexId = mMuDst->currentVertexIndex();
      StMuPrimaryVertex *selectedVertex = nullptr;
      mMuDst->setVertexIndex(0);
      selectedVertex = mMuDst->primaryVertex();
      if (!selectedVertex)
      {
         LOG_INFO << "Vertex is not valid" << endm;
         mMuDst->setVertexIndex(originalVertexId);
      }
      // end of vertex selection
      //------------------------------------------------------------------------------

      if (!selectedVertex)
         continue;

      // Run20 Au+Au 9.2 GeV trigger
      if (!mMuEvent->triggerIdCollection().nominal().isTrigger(780020))
         continue;

      // Vz range from StRefMultCorr Param.h: Run20 9.2 GeV = [-145, 145] cm
      if (fabs(mMuEvent->primaryVertexPosition().z()) > 145.0)
         continue;
      // Vr
      if (mMuEvent->primaryVertexPosition().perp() > 2.0)
         continue;

      int Run = mMuEvent->runId();
      refmultCorrUtil->init(Run);
      if (refmultCorrUtil->isBadRun(Run))
         continue;
      refmultCorrUtil->initEvent(mMuEvent->refMult(), mMuEvent->primaryVertexPosition().z(), 0);
      int Centrality = 1 + refmultCorrUtil->getCentralityBin9();
      histRefMult->Fill(refmultCorrUtil->getRefMultCorr());
      if (cen && cen != Centrality)
         continue;
      histRefMult2->Fill(refmultCorrUtil->getRefMultCorr());

      // fill MC histograms
      TClonesArray *MuMcVertices = mMuDst->mcArray(0);
      Int_t NoMuMcVertices = MuMcVertices->GetEntriesFast();
      TClonesArray *MuMcTracks = mMuDst->mcArray(1);
      Int_t NoMuMcTracks = MuMcTracks->GetEntriesFast();
      LOG_INFO << "# of MC tracks = " << NoMuMcTracks << " # of MC vertices = " << NoMuMcVertices << endm;
      if (!NoMuMcVertices || !NoMuMcTracks)
      {
         LOG_WARN << "Ev. " << i << " has no MC information ==> skip it" << endm;
         continue;
      }
      Int_t nMc = 0;

      // Loop for MC tracks
      for (Int_t itrk = 0; itrk < NoMuMcTracks; itrk++)
      {
         StMuMcTrack *mcTrack = (StMuMcTrack *)MuMcTracks->UncheckedAt(itrk);
         if (!mcTrack)
            continue;

         Int_t IdVx = mcTrack->IdVx();
         if (IdVx != 1)
            continue;

         const int Gid = mcTrack->GePid();

         nMc++;
         if (Gid == pid)
         {
            hPtMc->Fill(mcTrack->Pxyz().perp());
            hPhiMc->Fill(mcTrack->Pxyz().phi());
            hEtaMc->Fill(mcTrack->Pxyz().pseudoRapidity());
            if (fabs(mcTrack->Pxyz().pseudoRapidity()) < eta_cut)
            {
               hSelPtMc->Fill(mcTrack->Pxyz().perp());
               hSelPMc->Fill(mcTrack->Pxyz().mag());
               hSelPtEtaMc->Fill(mcTrack->Pxyz().perp(), mcTrack->Pxyz().pseudoRapidity());
            }
         }
         else
         {
            LOG_WARN << "Gid: " << Gid << " in Ev. " << i << endm;
         }
      }

      // primary tracks
      for (int i = 0; i < mMuDst->primaryTracks()->GetEntries(); i++)
      {
         StMuTrack *ptrack = mMuDst->primaryTracks(i);
         if (!ptrack) continue;
         if (ptrack->idTruth() <= 0 || ptrack->idTruth() > NoMuMcTracks)
            continue;
         StMuMcTrack *mcTrack = (StMuMcTrack *)MuMcTracks->UncheckedAt(ptrack->idTruth() - 1);
         if (!mcTrack)
         {
            LOG_WARN << "Inconsistency in mcArray(1), ignored" << endm;
            continue;
         }
         if (mcTrack->Id() != ptrack->idTruth())
         {
            LOG_WARN << "Mismatched idTruth " << ptrack->idTruth() << " and mcTrack Id " << mcTrack->Id()
                     << " this track is ignored" << endm;
            continue;
         }
         Int_t idMcVx = mcTrack->IdVx();
         while (idMcVx != 1)
         {
            StMuMcVertex *mcVertex = (StMuMcVertex *)MuMcVertices->UncheckedAt(idMcVx - 1);
            Int_t idMcTrack = mcVertex->IdParTrk();
            if (!idMcTrack)
               break;
            StMuMcTrack *mcTrackP = (StMuMcTrack *)MuMcTracks->UncheckedAt(idMcTrack - 1);
            idMcVx = mcTrackP->IdVx();
            if (!idMcVx)
               break;
         }
         if (idMcVx != 1)
            continue;
         if (ptrack->qaTruth() < 50.)
            continue;
         if ((mcTrack->GePid() != pid))
            continue;
         if (mcTrack->IdVx() != 1)
         {
            LOG_WARN << "mc track may not directly originate from PV!" << endm;
         }

         if (ptrack->nHitsFit() < 15)
            continue;
         if (ptrack->dcaGlobal().mag() > 3)
            continue;
         StThreeVectorF p = ptrack->p();
         if (abs(p.pseudoRapidity()) > eta_cut)
            continue;
         double nfitovermax = ptrack->nHitsFit() / float(ptrack->nHitsPoss());
         if (nfitovermax < 0.52 || nfitovermax > 1.05)
            continue;
         if (ptrack->nHitsDedx() < 15) continue;
         hnSigmaPt->Fill(p.perp(), ptrack->nSigmaProton());
         if (ptrack->flag() <= 0)
            continue;

         hPhi->Fill(p.phi());
         hPt->Fill(p.perp());
         hEta->Fill(p.pseudoRapidity());
         hSelPt->Fill(p.perp());
         hSelP->Fill(p.mag());
         hSelPtEta->Fill(p.perp(), p.pseudoRapidity());
         hEnergyLostPt->Fill(p.perp(), p.perp() - mcTrack->Pxyz().perp());
         hEnergyLostMCPt->Fill(mcTrack->Pxyz().perp(), p.perp() - mcTrack->Pxyz().perp());
         hEnergyLostPtEta->Fill(p.perp(), p.pseudoRapidity(), p.perp() - mcTrack->Pxyz().perp());
         hEnergyLostMCPtEta->Fill(mcTrack->Pxyz().perp(), p.pseudoRapidity(), p.perp() - mcTrack->Pxyz().perp());
         hSmearPx->Fill(p.x(), p.x() - mcTrack->Pxyz().x());
         hSmearPy->Fill(p.y(), p.y() - mcTrack->Pxyz().y());
         hSmearPz->Fill(p.z(), p.z() - mcTrack->Pxyz().z());
         hSmearPxEta->Fill(p.x(), p.pseudoRapidity(), p.x() - mcTrack->Pxyz().x());
         hSmearPyEta->Fill(p.y(), p.pseudoRapidity(), p.y() - mcTrack->Pxyz().y());
         hSmearPzEta->Fill(p.z(), p.pseudoRapidity(), p.z() - mcTrack->Pxyz().z());
      }

      // global tracks
      TObjArray *tracks_global = muDstMaker->muDst()->globalTracks();
      TObjArrayIter GetTracks_global(tracks_global);
      StMuTrack *gtrack;
      while ((gtrack = (StMuTrack *)GetTracks_global.Next()))
      {
         if (gtrack->idTruth() <= 0 || gtrack->idTruth() > NoMuMcTracks) continue;
         StMuMcTrack *mcTrack = (StMuMcTrack *)MuMcTracks->UncheckedAt(gtrack->idTruth() - 1);
         if (!mcTrack)
         {
            LOG_WARN << "Inconsistency in mcArray(1), ignored" << endm;
            continue;
         }
         if (mcTrack->Id() != gtrack->idTruth())
         {
            LOG_WARN << "Mismatched idTruth " << gtrack->idTruth() << " and mcTrack Id " << mcTrack->Id()
                     << " this track is ignored" << endm;
         }
         if ((mcTrack->GePid() != pid))
            continue;
         if (mcTrack->IdVx() != 1)
         {
            LOG_WARN << "mc track may not directly originate from PV!" << endm;
         }
         if (gtrack->qaTruth() < 50.)
            continue;

         if (gtrack->nHitsFit() < 15)
            continue;
         StThreeVectorF p = gtrack->p();
         if (abs(p.pseudoRapidity()) > eta_cut)
            continue;
         double nfitovermax = gtrack->nHitsFit() / float(gtrack->nHitsPoss());
         if (nfitovermax < 0.52 || nfitovermax > 1.05)
            continue;
         if (gtrack->nHitsDedx() < 15) continue;
         hnSigmaPt->Fill(p.perp(), gtrack->nSigmaProton());
         if (gtrack->flag() <= 0)
            continue;

         if (!gtrack->primaryTrack()) hSelFirstPointDis->Fill(p.perp(), (gtrack->firstPoint() - mMuEvent->primaryVertexPosition()).mag());
         if (!gtrack->primaryTrack()) hSelLastPointDis->Fill(p.perp(), (gtrack->lastPoint() - mMuEvent->primaryVertexPosition()).mag());
      }
   }
   hEffPt->Divide(hSelPt, hSelPtMc, 1, 1); //"B"
   if (nEvents > 1)
      chain->Finish();

   if (tags_output != NULL)
      tags_output->Write();
   if (tags_output != NULL)
      tags_output->Close();
   delete tags_output;
   delete chain;
}

