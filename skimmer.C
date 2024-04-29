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
#include "Setup/skimmerHelper.h"

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
  
  //Other custom counters can be initialized here.
  
  //_HstFile = new TFile(_HstFileName,"recreate");
  
  BookHistograms();
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
  cout<<"nEvtPass = "<<nEvtPass<<" ("<<passevtfrac*100<<" %)"<<endl;
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

  fReader.SetLocalEntry(entry);
  if(_data == 0)
    fReader_MC.SetLocalEntry(entry);
  if(_data == 1)
    fReader_Data.SetLocalEntry(entry);
  
  //Verbosity determines the number of processed events after which the root prompt is supposed to display a status update.
  if(_verbosity==0 && nEvtTotal%1000000==0)cout<<"Processed "<<nEvtTotal<<" event..."<<endl;      
  else if(_verbosity>0 && nEvtTotal%1000000==0)cout<<"Processed "<<nEvtTotal<<" event..."<<endl;

  nEvtTotal++;         //Total number of events containing everything (including the trash events).

  //ReadBranch(); //For skimmer
  
  //h.nevt->Fill(0);
  
  //The following flags throws away some events based on unwanted properties (such as detector problems)
  GoodEvt2018 = (_year==2018 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  GoodEvt2017 = (_year==2017 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  GoodEvt2016 = (_year==2016 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  
  GoodEvt = GoodEvt2018 && GoodEvt2017 && GoodEvt2016;                          
  
  if(GoodEvt){
    nEvtGood++;                         //Total number of events containing goodEvents. The analysis is done for these good events.

    //h.nevt->Fill(1);

    triggerRes=true; //Always true for MC


    if(_data==1){
      triggerRes = false;
      bool muon_trigger = false;
      bool electron_trigger = false;
      if     (_year==2016) {muon_trigger = (*HLT_IsoMu24==1); electron_trigger = (*HLT_Ele27_WPTight_Gsf==1);}
      else if(_year==2017) {muon_trigger = (*HLT_IsoMu27==1); electron_trigger = (*HLT_Ele32_WPTight_Gsf==1);}
      else if(_year==2018) {muon_trigger = (*HLT_IsoMu24==1); electron_trigger = (*HLT_Ele27_WPTight_Gsf==1);}

      //Muons are preferrred over electrons.
      //For the electron dataset, pick up only those events which do not fire a Muon trigger.
      //Otherwise there will be overcounting.
      
      triggerRes = muon_trigger || (!muon_trigger && electron_trigger);
      //triggerRes = electron_trigger || (!electron_trigger && muon_trigger);
    }


    if(triggerRes){
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

      
      //----------------------------------------------------------------------------------------------------------
      //                                        EVENT SELECTION                                                   |
      //-----------------------------------------------------------------------------------------------------------
      

      //Applying trigger to MC  
      bool single_muon = false;
      bool single_electron = false;
 
      if((int)recoMuon.size()>0 && recoMuon.at(0).v.Pt()>24)            single_muon = true;
      if((int)recoElectron.size()>0 && recoElectron.at(0).v.Pt()>27)    single_electron = true;
       
      bool triggered_events = false;
      //If the event has a single muon passing the trigger, keep it.
      if(single_muon) triggered_events=true;
      //If the event does not pass the single muon trigger then check for the single electron trigger, if it does then keep the event.
      else if(!single_muon && single_electron) triggered_events=true;    

      /*
	if(single_electron) triggered_events=true;
	else if(!single_electron && single_muon) triggered_events=true;
      */


      if(triggered_events){
      

	//---------------------------------------------------------------------------------------------------------------------
	//                                             SKIMMING STARTS                                                        |
	//---------------------------------------------------------------------------------------------------------------------

	//bool is_2l_event = false;
	//if((int)lightLep.size()>1) is_2l_event = true;
      
	if((int)recoLepton.size()>2){
	  nEvtSkim++;
	  skimTree->Fill();
	}
      
      }//triggered_events

    }//triggerRes
  
  }//GoodEvt

  return kTRUE;
}

void skimmer::BookHistograms(){
  
}


