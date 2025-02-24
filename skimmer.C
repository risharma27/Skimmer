#define skimmer_cxx

#include "skimmer.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

//Including the header files
#include "Setup/CustomFunctions.h"
#include "Setup/ProduceGenCollection.h"
#include "Setup/ProduceRecoCollection.h"
//#include "Setup/skimmerHelper.h"

void skimmer::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();
}

void skimmer::SlaveBegin(TTree * /*tree*/)
{
  time(&start);
  
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();
  
  //Initialization of the counters:
 
  nEvtTotal    = 0;
  nEvtGood     = 0;
  nEvtTrigger  = 0;
  nEvtPass     = 0;
  nEvtSkim     = 0;

  //For skimmer
  //tree->SetBranchStatus("*",1);
  //ActivateBranch(tree);
  
  //Other custom counters can be initialized here.
  
  //_HstFile = new TFile(_HstFileName,"recreate");
  
}

void skimmer::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
  
  // _HstFile->Write();
  //_HstFile->Close();

  //For Skimmer
  cout<<"Writing Skim File..."<<endl;
  skimTree->Write();
  _SkimFile->Close();
  cout<<" Done! "<<endl;

  float goodevtfrac = ((float)nEvtGood)/((float)nEvtTotal);
  float trigevtfrac = ((float)nEvtTrigger)/((float)nEvtTotal);
  float passevtfrac = ((float)nEvtPass)/((float)nEvtTotal);

  //The following lines are displayed on the root prompt.
  
  cout<<"---------------------------------------------"<<endl;
  cout<<"Summary:"<<endl;
  cout<<"nEvtTotal = "<<nEvtTotal<<endl;
  cout<<"nEvtGood = "<<nEvtGood<<" ("<<goodevtfrac*100<<" %)"<<endl;
  cout<<"nEvtTrigger = "<<nEvtTrigger<<" ("<<trigevtfrac*100<<" %)"<<endl;
  cout<<"nEvtSkim = "<<nEvtSkim<<endl;
  cout<<"---------------------------------------------"<<endl;

  //The following lines are written on the sum_<process name>.txt file
  ofstream fout(_SumFileName);
  fout<<"Total events ran = "<<nEvtTotal<<endl;
  fout<<"Total good events  = "<<nEvtGood<<endl;
  fout<<"Total events after skimming = "<<nEvtSkim<<endl;

  time(&end);

  double time_taken = double(end-start);
  cout<<"Time taken by the program="<<fixed<<time_taken<<setprecision(5);
  cout<<"sec"<<endl;

}

void skimmer::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file. 
}

Bool_t skimmer::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // When processing keyed objects with PROOF, the object is already loaded
  // and is available via the fObject pointer.
  //
  // This function should contain the \"body\" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.

  //------------------------------------------------------
  //Initializing fReaders:
  fReader.SetLocalEntry(entry);
  if(_run3)  fReader_Run3.SetLocalEntry(entry);
  else       fReader_Run2.SetLocalEntry(entry);
  if(_data == 0){
    fReader_MC.SetLocalEntry(entry);
    if(!_run3) fReader_Run2_MC.SetLocalEntry(entry);
    else       fReader_Run3_MC.SetLocalEntry(entry);
  }
  if(_year==2017)  fReader_2017.SetLocalEntry(entry);
  //-------------------------------------------------------
  
  //Verbosity determines the number of processed events after which the root prompt is supposed to display a status update.
  if(_verbosity==0 && nEvtTotal%1000000==0)cout<<"Processed "<<nEvtTotal<<" event..."<<endl;      
  else if(_verbosity>0 && nEvtTotal%1000000==0)cout<<"Processed "<<nEvtTotal<<" event..."<<endl;

  nEvtTotal++;         //Total number of events containing everything (including the trash events).

  //ReadBranch(); //For skimmer
  
  //h.nevt->Fill(0);
  
  //The following flags throws away some events based on unwanted properties (such as detector problems)
  GoodEvt2018 = (_year==2018 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && *Flag_BadPFMuonFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  GoodEvt2017 = (_year==2017 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && *Flag_BadPFMuonFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  GoodEvt2016 = (_year==2016 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && *Flag_BadPFMuonFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);

  GoodEvt2022 = (_year==2022 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && *Flag_BadPFMuonFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  
  GoodEvt = GoodEvt2018 && GoodEvt2017 && GoodEvt2016;
  GoodEvt = GoodEvt2022;
  
  //if(GoodEvt){
  if(true){
    nEvtGood++;                         //Total number of events containing goodEvents. The analysis is done for these good events.

    //h.nevt->Fill(1);

    triggerRes=true; //Always true for MC

    if(_data==1){
      triggerRes = false;
      
      bool muon_trigger = false;
      bool electron_trigger = false;
      bool overlapping_events = false;

      //Important
      //Declare all the HLT paths that you are using or are of interest here,
      //otherwise they won't get filled in the skimmed tree.
      
      if     (_year==2016) {muon_trigger = (*HLT_IsoMu24==1); electron_trigger = (*HLT_Ele27_WPTight_Gsf==1);}
      else if(_year==2017) {muon_trigger = (*HLT_IsoMu27==1); electron_trigger = (*HLT_Ele32_WPTight_Gsf==1);}
      else if(_year==2018) {muon_trigger = (*HLT_IsoMu24==1); electron_trigger = (*HLT_Ele27_WPTight_Gsf==1);}
      else if(_year==2022) {muon_trigger = (*HLT_IsoMu24==1); electron_trigger = (*HLT_Ele27_WPTight_Gsf==1);}

      //if(overlapping_events == true) cout<<"yes"<<endl;
      
      //Muons are preferrred over electrons.
      //For the electron dataset, pick up only those events which do not fire a Muon trigger.
      //Otherwise there will be overcounting.

      triggerRes = muon_trigger || electron_trigger;
      
      if(_flag == "electron_dataset" && overlapping_events) triggerRes = false;

      //Alternative way
      //if(_flag != "electron_dataset") triggerRes = muon_trigger && !electron_trigger; //For the SingleMuon dataset
      //if(_flag == "electron_dataset") triggerRes = electron_trigger && !muon_trigger; //For the SingleElectron dataset  

      //for displaced Muon TnP studies
      triggerRes = (*HLT_Mu7p5_L2Mu2_Jpsi);
      triggerRes = (*HLT_DoubleMu4_3_Bs);
      triggerRes = (*HLT_DoubleMu4_Jpsi_Displaced);
      triggerRes = (*HLT_DoubleMu4_Jpsi_NoVertexing);

      //displaced triggers (which could be of interest later)
      triggerRes = (*HLT_Mu20NoFiltersNoVtxDisplaced_Photon20_CaloCustomId);
      triggerRes = (*HLT_Mu38NoFiltersNoVtxDisplaced_Photon38_CaloIdL);
      triggerRes = (*HLT_DoubleL2Mu23NoVtx_2Cha);
      triggerRes = (*HLT_DoubleL2Mu25NoVtx_2Cha);	
      triggerRes = (*HLT_DoubleL3Mu16_10NoVtx_DxyMin0p01cm);
      triggerRes = (*HLT_DoubleL3Mu18_10NoVtx_DxyMin0p01cm);
      triggerRes = (*HLT_DoubleL3Mu20_10NoVtx_DxyMin0p01cm);
      triggerRes = (*HLT_DoubleL2Mu10NoVtx_2Cha_VetoL3Mu0DxyMax1cm);
      triggerRes = (*HLT_DoubleL2Mu12NoVtx_2Cha_VetoL3Mu0DxyMax1cm);
      triggerRes = (*HLT_DoubleL2Mu14NoVtx_2Cha_VetoL3Mu0DxyMax1cm);
    }

    //if(triggerRes){
    if(true){  //I am also keeping triggerRes=true in data (also keeping the overcounted events) so as to have flexibilty later.
      nEvtTrigger++; //Total number of events that pass the trigger
      //h.nevt->Fill(2);

      //#################################################################//
      //                         RecoParticle Block                      //
      //#################################################################//

      recoMuon.clear();
      recoElectron.clear();
      recoLepton.clear();
      recoJet.clear();
      bJet.clear();
    
      RecoLeptonArray();
      RecoJetArray();
      SecondaryVtxArray();
      OtherPrimaryVtxArray();

      Sortpt(recoMuon);
      Sortpt(recoElectron);
      Sortpt(recoLepton);
      Sortpt(recoJet);
      Sortpt(bJet);


      if(_data==0){
	genMuon.clear();
	genElectron.clear();

	GenLeptonArray();

	Sortpt(genMuon);
	Sortpt(genElectron);
      }
      
      //initializing MET variables
      metpt  = *MET_pt;
      metphi = *MET_phi;
      puppi_metpt  = *PuppiMET_pt;
      puppi_metphi = *PuppiMET_phi;

      //initializing PV variables
      PVx        = *PV_x;
      PVy        = *PV_y;
      PVz        = *PV_z;
      PVchi2     = *PV_chi2;
      PVndof     = *PV_ndof;
      PVscore    = *PV_score;
      PVnpvs     = *PV_npvs;
      PVnpvsGood = *PV_npvsGood;
      
      //---------------------------------------------------------------------------------------------------------------------
      //                                             SKIMMING STARTS                                                        |
      //---------------------------------------------------------------------------------------------------------------------

      if((int)recoLepton.size()>2){ //3L inclusive
	bool trigger = false;
	if(abs(recoLepton.at(0).id)==11 && recoLepton.at(0).v.Pt()>30) trigger = true;
	if(abs(recoLepton.at(0).id)==13 && recoLepton.at(0).v.Pt()>26) trigger = true;
      
	//if(trigger){
	if(true){
	  nEvtSkim++;
	  skimTree->Fill();
	}
      }
      
    }//triggerRes
    
  }//GoodEvt
  
  return kTRUE;
}

