// Reco Leptons (Electrons and Muons) Collection //

void skimmer::RecoLeptonArray(){
  
  //Muons

  float metpt  = *MET_pt;
  float metphi = *MET_phi;
  
  int nmu = 0;                       
  for(int i=0; i<(*nMuon); i++){
    Lepton temp;                      
    temp.v.SetPtEtaPhiM(Muon_pt[i],Muon_eta[i],Muon_phi[i],0.105); //the muon mass in GeV is 0.105
    temp.id = -13*Muon_charge[i];    //pdgID for mu- = 13, pdgID for mu+ = -13  
    temp.ind = i;
    temp.ip3d = Muon_ip3d[i];
    temp.sip3d = Muon_sip3d[i];
    temp.reliso03 = Muon_pfRelIso03_all[i];
    temp.reliso04 = Muon_pfRelIso04_all[i];
    temp.dxy = Muon_dxy[i];
    temp.dz = Muon_dz[i];
    
    bool ptetacut = temp.v.Pt()>10 && fabs(temp.v.Eta())<2.4;
    //bool passcut_mediummu =  ptetacut && Muon_pfRelIso03_all[i]<0.15 && Muon_mediumId[i];
    bool passcut_loosemu =  ptetacut && Muon_looseId[i];
    bool passcut_mediummu = ptetacut && Muon_mediumId[i];
    bool passcut_tightmu =  ptetacut && Muon_tightId[i];
         
    if(ptetacut){
      recoMuon.push_back(temp);
      recoLepton.push_back(temp);
    }

  } //Muons


  //Electrons
  int nel=0;
  for (int i = 0; i < (*nElectron); i++) {
    
    Lepton temp;
    temp.v.SetPtEtaPhiM(Electron_pt[i],Electron_eta[i],Electron_phi[i],Electron_mass[i]);
    temp.id = -11*Electron_charge[i];
    temp.ind = i;
    temp.ip3d = Electron_ip3d[i];
    temp.sip3d = Electron_sip3d[i];
    temp.reliso03 = Electron_pfRelIso03_all[i]; 
    temp.dxy = Electron_dxy[i];
    temp.dz = Electron_dz[i];
	    
    bool ptetacut = temp.v.Pt()>10 && fabs(temp.v.Eta())<2.4;
    //bool passcut_mediumel = ptetacut && Electron_pfRelIso03_all[i]<0.15 && Electron_cutBased[i]>1;
    //bool passcut_mediumel = ptetacut && Electron_mvaFall17V2noIso_WP80[i];
    Int_t bitmap = Electron_vidNestedWPBitmap[i];
    //bool passcut_mediumel = ptetacut && electronCustomID(bitmap, 3, 7)==1;
    bool passcut_mediumel = ptetacut && Electron_cutBased[i]>2;
    
    if(ptetacut){
      recoElectron.push_back(temp);
      recoLepton.push_back(temp);
    }

  } //Electrons
    
}


// Jets Collection //

void skimmer::RecoJetArray(){
  for(int i=0; i< (*nJet); i++){
    Lepton temp;
    temp.v.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
    temp.ind = i;

    bool ptetacut = temp.v.Pt()>30 && fabs(temp.v.Eta())<2.4;
    bool cleaned_from_leptons = clean_from_array(temp, recoLepton, 0.4);
    bool jetID = _year == 2016 ? Jet_jetId[i]>=1 : Jet_jetId[i]>=2; //if 2016, >=1; else >=2
    bool passcut = ptetacut && cleaned_from_leptons && jetID;

    if(passcut){
      recoJet.push_back(temp);
      //if(Jet_btagDeepB[i]>0.4184) bJet.push_back(temp); //Run2
      if(Jet_btagDeepFlavB[i]>0.8) bJet.push_back(temp); //Run3
    }
  }

}

void skimmer::SecondaryVtxArray(){
  for(int i=0; i< (*nSV); i++){
    Lepton temp;
    temp.v.SetPtEtaPhiM(SV_pt[i],SV_eta[i],SV_phi[i],SV_mass[i]);
    temp.ind = i;
    temp.charge = SV_charge[i];
    temp.dxy = SV_dxy[i];
    temp.dxySig = SV_dxySig[i];
    temp.dlen = SV_dlen[i];
    temp.dlenSig = SV_dlenSig[i];
    temp.vtx_x = SV_x[i];
    temp.vtx_y = SV_y[i];
    temp.vtx_z = SV_z[i];
    temp.vtx_ntracks = SV_ntracks[i];
    temp.vtx_ndof = SV_ndof[i];
    temp.chi2 = SV_chi2[i];    
    
  }
   
}

void skimmer::OtherPrimaryVtxArray(){
  for(int i=0; i< (*nOtherPV); i++){
    Lepton temp;
    temp.ind = i;
    temp.otherPVz = OtherPV_z[i];        
  }
   
}
