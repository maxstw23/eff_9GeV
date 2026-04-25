//wrapper to makeMuDstQA.C
//for ACLiC mode
#include "TROOT.h"
#include "TSystem.h"
#include "TString.h"

class StRefMultCorr;
class CentralityMaker;


void runmakeMuDstQA(TString InputFileList,Int_t cen, Int_t nfiles,Int_t nevents,TString OutputFile, Int_t pid=8)
{
   gROOT->Reset();
   gROOT->Macro("loadMuDst.C");
  gSystem->Load("StRefMultCorr");
/*  TString str;
  str = ".x makeMuDstQA.C+(\"";
  str += InputFileList;
  str += "\",";
  str += cen;
  str += ",";
  str += nfiles;
  str += ",";
  str += nevents;
  str += ",\"";
  str += OutputFile;
  str += "\")";
gROOT->ProcessLine( str.Data() );
*/

  gROOT->LoadMacro("makeMuDstQA.C+");//+
   makeMuDstQA(InputFileList,cen,nfiles,nevents,OutputFile,pid);
}
