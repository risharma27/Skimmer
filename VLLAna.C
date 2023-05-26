#define VLLAna_cxx
#include "VLLAna.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

#include "IISERLogo.h"
#include "skimmerHelper.h"


void VLLAna::Begin(TTree * /*tree*/)
{
  
  TString option = GetOption();

  //NEW MVA TTree
  //InitializeMVATreeBranch();
}

void VLLAna::SlaveBegin(TTree *tree /*tree*/)
{
  time(&start);
  
  //   cout<<"Inside SlaveBegin()"<<endl;
  TString option = GetOption();
  nEvtTotal = 0;
  nEvtRan = 0;
  nEvtTrigger=0;
  nEvtSkim=0;
  
  //Create the histogram file
  _HstFile = new TFile(_HstFileName,"recreate");
  //Call the function to book the histograms we declared in Hists.
  BookHistograms();

  //For skimmer
  tree->SetBranchStatus("*",0);
  ActivateBranch(tree);

   
}
void VLLAna::SlaveTerminate()
{
  //   cout<<"Inside SlaveTerminate()"<<endl;
  //Write histograms and close histogram file
  //  _mvaFile->Write();
  //  _mvaFile->Close();
  _HstFile->Write();
  _HstFile->Close();
  //cout<<"Writing MVA Tree...."<<endl;

  //For skimmer
  cout<<"Writing Skim File....";
  skimTree->Write();
  _SkimFile->Close();
  cout<<"   Done!  "<<endl;
  
  //tree->Write();
  //Output to screen.
  cout<<"Total events in the file = "<<nEvtTotal<<endl;
  cout<<"Total events the code run on = "<<nEvtRan<<endl;
  cout<<"Total events that passed the trigger = "<< nEvtTrigger <<endl;
  cout<<"Total events after skimming = "<< nEvtSkim <<endl;
  
  //Open the text output file
  ofstream fout(_SumFileName);
  //Put text output in the summary file.
  fout<<"Total events in the file = "<<nEvtTotal<<endl;
  fout<<"Total events the code run on = "<<nEvtRan<<endl;
  fout<<"Total events that passed the trigger = "<< nEvtTrigger <<endl;
  fout<<"Total events after skimming = "<< nEvtSkim <<endl;

  time(&end);

  double time_taken = double(end-start);
  cout<<"Time taken by the program is= "<<fixed<<time_taken<<setprecision(5);
  cout<<"sec"<<endl;

  
}


void VLLAna::Terminate()
{
  //   cout<<"Inside Terminate()"<<endl;
}

Bool_t VLLAna::Process(Long64_t entry)
{
  fReader.SetLocalEntry(entry);
  if(_data == 0)
    fReader_MC.SetLocalEntry(entry);
  if(_data == 1)
    fReader_Data.SetLocalEntry(entry);
  
  //Verbosity determines the number of processed events after which the root prompt is supposed to display a status update.
  if(_verbosity==0 && nEvtTotal%10000==0)cout<<"Processed "<<nEvtTotal<<" event..."<<endl;      
  else if(_verbosity>0 && nEvtTotal%10000==0)cout<<"Processed "<<nEvtTotal<<" event..."<<endl;
  
  nEvtTotal++;
  h.nevt->Fill(0);
  
  //For skimmer:
  ReadBranch();
  //nEvtSkim++;
  //skimTree->Fill();  

  GoodEvt2018 = (_year==2018 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && *Flag_BadPFMuonFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  GoodEvt2017 = (_year==2017 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && *Flag_BadPFMuonFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  GoodEvt2016 = (_year==2016 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && *Flag_BadPFMuonFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  
  
  GoodEvt = GoodEvt2018 && GoodEvt2017 && GoodEvt2016;

  if(GoodEvt){
    
    nEvtRan++; //only good events
    h.nevt->Fill(1);
    
    
    triggerRes=true; //Always true for MC
    if(_data==1){
      trigger2018 = (_year==2018 ? (_lep==1 ? *HLT_IsoMu24==1 : _lep==0 && *HLT_Ele32_WPTight_Gsf) : 1);
      //trigger2017 = (_year==2017 ? (_lep==1 ? *HLT_IsoMu27==1 : _lep==0 && (*HLT_Ele32_WPTight_Gsf||*HLT_Ele32_WPTight_Gsf_L1DoubleEG)) : 1);
      trigger2017 = (_year==2017 ? (_lep==1 ? *HLT_IsoMu27==1 : _lep==0 && (*HLT_Ele32_WPTight_Gsf)) : 1);
      trigger2016 = (_year==2016 ? (_lep==1 ? (*HLT_IsoMu24==1) : _lep==0 && *HLT_Ele27_WPTight_Gsf) : 1);
      
      
      triggerRes = trigger2018 && trigger2017 && trigger2016;
      
    }
    if(triggerRes){
      nEvtTrigger++; //only triggered events
      h.nevt->Fill(2);
      
      
      //-----------------------------------------------------------------------------------------------------------------------
      //                                              GEN OBJECTS BLOCK BEGINS                                                |
      //-----------------------------------------------------------------------------------------------------------------------

      if(_data==0){
	
	genMuon.clear();
	
	for(unsigned int i=0; i< (*nGenPart); i++){
	  Lepton temp; temp.v.SetPtEtaPhiM(GenPart_pt[i],GenPart_eta[i],GenPart_phi[i],GenPart_mass[i]);
	  temp.status = GenPart_status[i]; temp.ind = i; temp.pdgid = GenPart_pdgId[i]; //temp.momid=MotherID(i,GenPart_genPartIdxMother[i]);
	  bool passcutmu= abs(temp.pdgid)==13 && temp.status==1 && temp.v.Pt()>5 && fabs(temp.v.Eta())<2.4;
	  
	  if(passcutmu){     	
	    genMuon.push_back(temp);
	  }
	}

	Sortpt(genMuon);
	
	genElectron.clear();
	
	for(unsigned int i=0; i<(*nGenPart); i++){
	  Lepton temp; temp.v.SetPtEtaPhiM(GenPart_pt[i],GenPart_eta[i],GenPart_phi[i],GenPart_mass[i]);
	  temp.status = GenPart_status[i]; temp.ind = i; temp.pdgid = GenPart_pdgId[i]; //temp.momid=MotherID(i,GenPart_genPartIdxMother[i]);
	  bool passcutel = abs(temp.pdgid)==11 && temp.status==1 && temp.v.Pt()>5 && fabs(temp.v.Eta())<2.4;
	  
	  if(passcutel){
	    genElectron.push_back(temp);
	  }
	}
	
	Sortpt(genElectron);
	  
      }    
      
      
      //-----------------------------------------------------------------------------------------------------------------------
      //                                              RECO OBJECTS BLOCK BEGINS                                                |
      //-----------------------------------------------------------------------------------------------------------------------
      
      RecoLep.clear();
      RecoMuon.clear();
      
      int nmu=0;
      
      for(unsigned int i=0; i<(*nMuon); i++){
	Lepton temp; temp.v.SetPtEtaPhiM(Muon_pt[i],Muon_eta[i],Muon_phi[i],0.105);
	temp.id = -13*Muon_charge[i]; temp.ind = i; temp.charge = Muon_charge[i];
	
	bool is_promptmuon= fabs(Muon_dxy[i])<0.05 && fabs(Muon_dz[i])<0.1;
	bool passcutmu = temp.v.Pt()>10 && fabs(temp.v.Eta())<2.4 && Muon_mediumId[i] && Muon_pfRelIso04_all[i]<0.25;
	
	bool analysisCut = passcutmu && is_promptmuon;
	
	if(analysisCut){
	  nmu++;
	  RecoMuon.push_back(temp);
	  RecoLep.push_back(temp);
	}
      }
      
      Sortpt(RecoMuon);
      
      RecoElectron.clear();
      int nelec=0;
      
      for(unsigned int i=0; i<(*nElectron); i++){
	Lepton temp; temp.v.SetPtEtaPhiM(Electron_pt[i],Electron_eta[i],Electron_phi[i],0.000511); 
	temp.id = -11*Electron_charge[i]; temp.ind = i; temp.charge = Electron_charge[i];
	
	bool is_promptelectron = false;
	if(fabs(temp.v.Eta())<=1.479){
	  if(fabs(Electron_dxy[i])<0.05 && fabs(Electron_dz[i])<0.1) is_promptelectron = true;
	}
	
	bool passcutel = temp.v.Pt()>10 && fabs(temp.v.Eta())<2.4;
	
	bool analysisCut = passcutel && is_promptelectron;
	
	if(analysisCut){
	  nelec++;
	  RecoElectron.push_back(temp);
	  RecoLep.push_back(temp);
	}
      }
      
      Sortpt(RecoElectron);
      Sortpt(RecoLep);
      
      /*==========================
	           MET
	===========================*/
      metpt = *MET_pt;
      metphi = *MET_phi;
      
      h.etmiss->Fill(metpt);//fill a histogram with the missing Et of the event.
      
      
      /*===============================
  	       Event Selection
	===============================*/
      
      
      if((int)RecoLep.size()>2){ //3 or more leptons
	bool passTrigger = false;
	if(abs(RecoLep.at(0).id)==13 && RecoLep.at(0).v.Pt()>26) //muon pT>26GeV
	  passTrigger = true;
	if(abs(RecoLep.at(0).id)==11 && RecoLep.at(0).v.Pt()>32) //electron pT>32GeV
	   passTrigger = true;
	
	if(passTrigger){
	  nEvtSkim++;
	  skimTree->Fill();
	}
      }
      
      
      
      
      //--------------------END OF EVENT ENTRY-----------------------------// 
    }
  }
  
  return kTRUE;
  
}


void VLLAna::Sortpt(vector<Lepton> vec)
{
  for(int i=0; i<(int)vec.size()-1; i++){
    for(int j=i+1; j<(int)vec.size(); j++){
	if( vec[i].v.Pt() < vec[j].v.Pt() ) swap(vec.at(i), vec.at(j));
    }
  }
}


void VLLAna::BookHistograms()
{

  h.nevt = new TH1F("nEvents","0-Total events, 1-Total events ran, 2-Total events with trigger applied",5,-1,4);
  h.etmiss= new TH1F("etmiss_beforeMTcut","Missing E_{T}",1000,0,1000); h.etmiss->Sumw2();
  

}//end of BOOK HISTOS
