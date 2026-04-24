#! /opt/star/bin/perl -w

use File::Basename;
use Getopt::Std;
use Cwd 'abs_path';     # aka realpath()

my %opt; 
#my $cen = 6;
getopts('htoz:b:m:adn:q:x:y:s',\%opt);
my $usage = "$0  combinedDir \n";
#----

my $combineMacroName="myCombine";

 open ( combineMacro, ">$combineMacroName.C");

 print combineMacro  "#include \"TObject.h\"  \n";
 print combineMacro  "#include \"TObjArray.h\" \n";
 print combineMacro  "#include \"TKey.h\" \n";
 print combineMacro  "#include \"TH1.h\" \n";
 print combineMacro  "#include \"TFile.h\" \n";
 print combineMacro  "#include \"TList.h\" \n";
 print combineMacro  "#include \"TObjString.h\" \n";
 print combineMacro  "#include \<iostream\> \n";
 print combineMacro  "void $combineMacroName() {  \n";
 print combineMacro  "  TObjArray* toBeAddedFiles = new TObjArray(); \n";

$m=1;
#foreach $eachStdHistFile ( glob("../P19ib/cen4/*.wei*root") ) {
#foreach $eachStdHistFile ( glob("P19ib/cen0/*.v2*root") ) {
foreach $eachStdHistFile ( glob("*.root") ) {

chomp($eachStdHistFile);
	print combineMacro  "toBeAddedFiles->AddLast((TObject *)(new TObjString(\"$eachStdHistFile\"))); \n";
	$m ++;
}



 print combineMacro  "   TFile* newfile = new TFile(\"ntuple.merge.root\",\"RECREATE\"); \n";

 print combineMacro  "   TFile* oldfile1 = new  TFile(((TObjString *)(toBeAddedFiles->At(0)))->GetName(),\"READ\"); \n";
 print combineMacro  "   TList* list = oldfile1->GetListOfKeys(); \n";
 print combineMacro  "      TIter next(list); \n";
 print combineMacro  "      TKey* key; \n";
 print combineMacro  "      TObject* obj; \n";
 print combineMacro  "   while ((key = (TKey*)next())) { \n";
 print combineMacro  "   TString tempStr(key->GetName()); \n";
 print combineMacro  "   cout<<tempStr<<endl; \n";
# print combineMacro  "   if (!tempStr.Contains(\"Day\") && !tempStr.Contains(\"hCentrality\") && !tempStr.Contains(\"hTally\") ) continue; \n";
 print combineMacro  "        obj = oldfile1->Get(key->GetName()); \n";
 print combineMacro  "     if (!obj) return; \n";
 print combineMacro  "   if(obj->IsA() == TDirectory::Class()){ \n";
 print combineMacro  "                        delete obj;   \n";
 print combineMacro  "                         obj = NULL;  \n";
 print combineMacro  "                         continue;    \n";
 print combineMacro  "                 }                   \n";
 print combineMacro  "     TObject* newobj = obj->Clone(); \n";
 print combineMacro  "  if (newobj->InheritsFrom( TH1::Class())) { \n";
# do not need to newobj->Reset(), because k begins with 1 below !!
 print combineMacro  " for (int k=1; k<toBeAddedFiles->GetEntries(); k++){ \n";
 print combineMacro  "    TFile* f =new TFile(((TObjString *)(toBeAddedFiles->At(k)))->GetName(), \"READ\"); \n";
 print combineMacro  "((TH1 *) newobj)->Scale(); \n";
 print combineMacro  "((TH1 *) newobj)->Add(((TH1 *)f->Get(key->GetName()))); \n";
 print combineMacro  "   delete f; \n";
 print combineMacro  "  } \n";
 print combineMacro  "           } \n";
 print combineMacro  "         newfile->cd(); \n";
# print combineMacro  "        newobj->Write(); \n";
 print combineMacro  "        newobj->Write(key->GetName(),TObject::kOverwrite | TObject::kSingleKey); \n";
 print combineMacro  "       delete newobj; \n";
 print combineMacro  "   } \n";
 print combineMacro  "   gDirectory->cd(); \n";
 print combineMacro  "  delete key; \n";
 print combineMacro  "    newfile->Write(); \n";
 print combineMacro  "    newfile->Close(); \n";
 print combineMacro  " } \n";

 close combineMacro;

# print `bsub  -u rexwg -q star_cas_short -L /bin/tcsh -J combine -o $combineMacroName.log root4star -q -b '$combineMacroName.C\+\+'`;



exit;


