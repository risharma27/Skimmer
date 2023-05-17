

// CUSTOM FUNCTIONS


float VLLAna::delta_phi(float phi1, float phi2)
{
  //Calculate the correct deltaPhi=phi1-phi2
  phi1 = TVector2::Phi_0_2pi(phi1); //Phi_0_2pi() is a member function of the class TVector2 which returns phi angle in the interval [0, 2*pi)
  phi2 = TVector2::Phi_0_2pi(phi2);
  float dphi = fabs(phi1 - phi2);
  if(dphi>TMath::Pi()) dphi = 2*TMath::Pi() - dphi;
  return dphi;
}

int VLLAna::MotherID(int partindex, int momindex)
{
  int parid =GenPart_pdgId[partindex];
  int momid = GenPart_pdgId[momindex];
  while(parid==momid){
    partindex=momindex;
    momindex=GenPart_genPartIdxMother[momindex];
    parid =GenPart_pdgId[partindex];
    momid = GenPart_pdgId[momindex];
  }
  return momid;
}
int VLLAna::IsGenMatched(int id, TLorentzVector t)
{
  int genmatch=0;
  
  //Light Lepton Matching//
  for(int i=0;i<(int)genllep.size();i++){    
    if(t.DeltaR(genllep.at(i).v)<0.2 && abs(id)==abs(genllep.at(i).pdgid)){
      genmatch=1;break;
    }
  }
  return genmatch;
}

int VLLAna::GenMatchedLepIndex(int id, TLorentzVector t)
{
  int genmatch=0,lepindex=-1.0; float dr=0.0,drmin=999.0;
  
  //Light Lepton Matching//
  for(int i=0;i<(int)genllep.size();i++){
    
    if(abs(id)==abs(genllep.at(i).pdgid)){
      dr=t.DeltaR(genllep.at(i).v);
      if(dr<drmin)drmin=dr;
      if(drmin<0.4){	
	genmatch=1;lepindex=i;break;	
      }
    }
  }
  //cout<<"LepIndex="<<lepindex<<endl;
  return lepindex;
}


float VLLAna::getInvMass(TLorentzVector a,TLorentzVector b)
{
  return (a+b).M();
}


float VLLAna::transv_mass(float lepE, float lepphi, float met, float metphi)
{
  float dphi = fabs(lepphi - metphi);
  if(dphi>TMath::Pi()) dphi = 2*TMath::Pi() - dphi;
  float mT = sqrt(2* lepE * met *(1-cos(dphi)));
  return mT;
}


