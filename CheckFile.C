void CheckFile() {


for(int i=1;i<57;i++) {
//cout<<i<<endl;
char name[200];
sprintf(name,"Jobs/cen1/%d.root",i);
TFile *f = new TFile(name);
if(!f->Get("hEffPt")) 
//cout<<i<<endl;
cout<<"star-submit-template -template submit_condor.xml -entities \"job="<<i<<"\""<<endl;

//cout<<"rm -rf "<<i<<".ntuple.merge.root"<<endl;

}




























}
