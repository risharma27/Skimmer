//#include <string>
using namespace std;
void VLLAna::ActivateBranch(TTree *t){
  //char activeBranch[] = {"run","luminosityBlock","event","HLT_IsoMu18","HLT_IsoMu20","HLT_IsoMu24","HLT_IsoMu27","HLT_IsoTkMu18","HLT_IsoTkMu20","HLT_IsoTkMu22","HLT_IsoTkMu24","HLT_IsoTkMu27","HLT_Ele32_WPTight_Gsf","HLT_Ele27_WPTight_Gsf","Flag_*","nMuon","Muon_*","nElectron","Electron_*","nTau","Tau_*","nJet","Jet_*","nGenPart","GenPart_*","nGenJet","GenJet_*","nGenVisTau","GenVisTau_*","MET_*","PuppiMET_*","btagWeight_DeepCSVB","GenMET_phi","GenMET_pt","nTrigObj","TrigObj_*"};
  
  //cout<<"No of branches="<<activeBranch.size()<<endl;
  //for(auto activeBranchName : activeBranch){t->SetBranchStatus(activeBranchName, 1);}
  for(auto activeBranchName : {"run","luminosityBlock","event","HLT_IsoMu20","HLT_IsoMu24","HLT_IsoMu27","HLT_Ele32_WPTight_Gsf","HLT_Ele27_WPTight_Gsf","Flag_*","nMuon","Muon_*","nElectron","Electron_*","nTau","Tau_*","nJet","Jet_*","MET_*","PuppiMET_*","nTrigObj","TrigObj_*", "nPhoton", "Photon_*"})
    t->SetBranchStatus(activeBranchName, 1);
  
  //Activate ONLY FOR MC
  if(_data==0){
    for(auto activeBranchName : {"nGenPart","GenPart_*","nGenJet","GenJet_*","nGenVisTau","GenVisTau_*","btagWeight_DeepCSVB","GenMET_phi","GenMET_pt"})
      t->SetBranchStatus(activeBranchName, 1);
  }
}



void VLLAna::ReadBranch(){
  //ONLY READ
  //Muon,Electron,Jets,Taus,MET,PuppiMET,GenJet,GenPart,GenVisTau,nTrigObj,Flags,HLT_Path
  // Rules:
  // TTreeReaderValue branch: *variable
  // TTreeReaderArray branch: variable[i] where i is looping over no of that object
  
  *run;
  *luminosityBlock;
  *event;

  //HLT;
  *HLT_IsoMu24;
  *HLT_IsoMu27;
  *HLT_Ele32_WPTight_Gsf;
  *HLT_Ele27_WPTight_Gsf;
  
  //Flags
  *Flag_HBHENoiseFilter;
  *Flag_HBHENoiseIsoFilter;
  *Flag_CSCTightHaloFilter;
  *Flag_CSCTightHaloTrkMuUnvetoFilter;
  *Flag_CSCTightHalo2015Filter;
  *Flag_globalTightHalo2016Filter;
  *Flag_globalSuperTightHalo2016Filter;
  *Flag_HcalStripHaloFilter;
  *Flag_hcalLaserEventFilter;
  *Flag_EcalDeadCellTriggerPrimitiveFilter;
  *Flag_EcalDeadCellBoundaryEnergyFilter;
  *Flag_ecalBadCalibFilter;
  *Flag_goodVertices;
  *Flag_eeBadScFilter;
  *Flag_ecalLaserCorrFilter;
  *Flag_trkPOGFilters;
  *Flag_chargedHadronTrackResolutionFilter;
  *Flag_muonBadTrackFilter;
  *Flag_BadChargedCandidateFilter;
  *Flag_BadPFMuonFilter;
  *Flag_BadPFMuonDzFilter;
  *Flag_hfNoisyHitsFilter;
  *Flag_BadChargedCandidateSummer16Filter;
  *Flag_BadPFMuonSummer16Filter;
  *Flag_trkPOG_manystripclus53X;
  *Flag_trkPOG_toomanystripclus53X;
  *Flag_trkPOG_logErrorTooManyClusters;
  *Flag_METFilters;

  //btagWeight
  if(_data==0)*btagWeight_DeepCSVB; //MC

  //PFMET
  *MET_MetUnclustEnUpDeltaX;
  *MET_MetUnclustEnUpDeltaY;
  *MET_covXX;
  *MET_covXY;
  *MET_covYY;
  *MET_phi;
  *MET_pt;
  *MET_significance;
  *MET_sumEt;
  *MET_sumPtUnclustered;
  if(_data==0){
    *MET_fiducialGenPhi;
    *MET_fiducialGenPt;
  }

  //PuppiMET
  *PuppiMET_phi;
  *PuppiMET_phiJERDown;
  *PuppiMET_phiJERUp;
  *PuppiMET_phiJESDown;
  *PuppiMET_phiJESUp;
  *PuppiMET_phiUnclusteredDown;
  *PuppiMET_phiUnclusteredUp;
  *PuppiMET_pt;
  *PuppiMET_ptJERDown;
  *PuppiMET_ptJERUp;
  *PuppiMET_ptJESDown;
  *PuppiMET_ptJESUp;
  *PuppiMET_ptUnclusteredDown;
  *PuppiMET_ptUnclusteredUp;
  *PuppiMET_sumEt;
  
  //Muon Branches
  *nMuon;
  for(unsigned int i=0;i<(*nMuon);i++){
    Muon_dxy[i];
    Muon_dxyErr[i];
    Muon_dxybs[i];
    Muon_dz[i];
    Muon_dzErr[i];
    Muon_eta[i];
    Muon_ip3d[i];
    Muon_jetPtRelv2[i];
    Muon_jetRelIso[i];
    Muon_mass[i];
    Muon_miniPFRelIso_all[i];
    Muon_miniPFRelIso_chg[i];
    Muon_pfRelIso03_all[i];
    Muon_pfRelIso03_chg[i];
    Muon_pfRelIso04_all[i];
    Muon_phi[i];
    Muon_pt[i];
    Muon_ptErr[i];
    Muon_segmentComp[i];
    Muon_sip3d[i];
    Muon_softMva[i];
    Muon_tkRelIso[i];
    Muon_tunepRelPt[i];
    Muon_mvaLowPt[i];
    Muon_mvaTTH[i];
    Muon_charge[i];
    Muon_jetIdx[i];
    Muon_nStations[i];
    Muon_nTrackerLayers[i];
    Muon_pdgId[i];
    Muon_tightCharge[i];
    Muon_fsrPhotonIdx[i];
    Muon_highPtId[i];
    Muon_highPurity[i];
    Muon_inTimeMuon[i];
    Muon_isGlobal[i];
    Muon_isPFcand[i];
    Muon_isStandalone[i];
    Muon_isTracker[i];
    Muon_jetNDauCharged[i];
    Muon_looseId[i];
    Muon_mediumId[i];
    Muon_mediumPromptId[i];
    Muon_miniIsoId[i];
    Muon_multiIsoId[i];
    Muon_mvaId[i];
    Muon_mvaLowPtId[i];
    Muon_pfIsoId[i];
    Muon_puppiIsoId[i];
    Muon_softId[i];
    Muon_softMvaId[i];
    Muon_tightId[i];
    Muon_tkIsoId[i];
    Muon_triggerIdLoose[i];
    if(_data==0){
      Muon_genPartIdx[i];
      Muon_genPartFlav[i];
    }
    Muon_cleanmask[i];    
  }

  //Electron branches
  
  *nElectron;
  for(unsigned int i=0; i<(*nElectron);i++){
    Electron_dEscaleDown[i];
    Electron_dEscaleUp[i];
    Electron_dEsigmaDown[i];
    Electron_dEsigmaUp[i];
    Electron_deltaEtaSC[i];
    Electron_dr03EcalRecHitSumEt[i];
    Electron_dr03HcalDepth1TowerSumEt[i];
    Electron_dr03TkSumPt[i];
    Electron_dr03TkSumPtHEEP[i];
    Electron_dxy[i];
    Electron_dxyErr[i];
    Electron_dz[i];
    Electron_dzErr[i];
    Electron_eCorr[i];
    Electron_eInvMinusPInv[i];
    Electron_energyErr[i];
    Electron_eta[i];
    Electron_hoe[i];
    Electron_ip3d[i];
    Electron_jetPtRelv2[i];
    Electron_jetRelIso[i];
    Electron_mass[i];
    Electron_miniPFRelIso_all[i];
    Electron_miniPFRelIso_chg[i];
    Electron_mvaFall17V2Iso[i];
    Electron_mvaFall17V2noIso[i];
    Electron_pfRelIso03_all[i];
    Electron_pfRelIso03_chg[i];
    Electron_phi[i];
    Electron_pt[i];
    Electron_r9[i];
    Electron_scEtOverPt[i];
    Electron_sieie[i];
    Electron_sip3d[i];
    Electron_mvaTTH[i];
    Electron_charge[i];
    Electron_cutBased[i];
    Electron_jetIdx[i];
    Electron_pdgId[i];
    Electron_photonIdx[i];
    Electron_tightCharge[i];
    Electron_vidNestedWPBitmap[i];
    Electron_vidNestedWPBitmapHEEP[i];
    Electron_convVeto[i];
    Electron_cutBased_HEEP[i];
    Electron_isPFcand[i];
    Electron_jetNDauCharged[i];
    Electron_lostHits[i];
    Electron_mvaFall17V2Iso_WP80[i];
    Electron_mvaFall17V2Iso_WP90[i];
    Electron_mvaFall17V2Iso_WPL[i];
    Electron_mvaFall17V2noIso_WP80[i];
    Electron_mvaFall17V2noIso_WP90[i];
    Electron_mvaFall17V2noIso_WPL[i];
    Electron_seedGain[i];
    if(_data==0){
      Electron_genPartIdx[i];
      Electron_genPartFlav[i];
    }
    Electron_cleanmask[i];
  }
  
  
  //Jets
  *nJet;
  for(unsigned int i=0; i<(*nJet);i++){
    Jet_area[i];
    Jet_btagCSVV2[i];
    Jet_btagDeepB[i];
    Jet_btagDeepCvB[i];
    Jet_btagDeepCvL[i];
    Jet_btagDeepFlavB[i];
    Jet_btagDeepFlavCvB[i];
    Jet_btagDeepFlavCvL[i];
    Jet_btagDeepFlavQG[i];
    Jet_chEmEF[i];
    Jet_chFPV0EF[i];
    Jet_chHEF[i];
    Jet_eta[i];
    Jet_hfsigmaEtaEta[i];
    Jet_hfsigmaPhiPhi[i];
    Jet_mass[i];
    Jet_muEF[i];
    Jet_muonSubtrFactor[i];
    Jet_neEmEF[i];
    Jet_neHEF[i];
    Jet_phi[i];
    Jet_pt[i];
    Jet_puIdDisc[i];
    Jet_qgl[i];
    Jet_rawFactor[i];
    Jet_bRegCorr[i];
    Jet_bRegRes[i];
    Jet_cRegCorr[i];
    Jet_cRegRes[i];
    Jet_electronIdx1[i];
    Jet_electronIdx2[i];
    Jet_hfadjacentEtaStripsSize[i];
    Jet_hfcentralEtaStripSize[i];
    Jet_jetId[i];
    Jet_muonIdx1[i];
    Jet_muonIdx2[i];
    Jet_nElectrons[i];
    Jet_nMuons[i];
    Jet_puId[i];
    Jet_nConstituents[i];
    if(_data==0){
      Jet_genJetIdx[i];
      Jet_hadronFlavour[i];
      Jet_partonFlavour[i];
    }
    Jet_cleanmask[i];
  }
}
