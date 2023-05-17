#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
void ana(int sample=0)
{
  const char *hstfilename, *sumfilename, *skimfilename;
  
  TChain *chain = new TChain("Events");
  VLLAna m_selec;//declared an instance of our class.
  
  //--------------------------------------------------------------------------//
  //               Choose the sample you want to run over                     //
  //--------------------------------------------------------------------------//
  
  if(sample==0){
    chain->Add("/home/riya/Riya/Work/Skimmer/inputs/DYJetsToLL_M-50.root");
    hstfilename = "outputs/hst_DYToLL.root";
    sumfilename = "outputs/sum_DYToLL.txt";
    skimfilename = "outputs/DYToLL_skimmed.root";
    m_selec.SetData(0); //0 - running over MC, 1 - running over Data
    m_selec.SetYear(2018);
    m_selec.SetMCwt(1);
    m_selec.SetLep(1); //0-electron dataset, 1-muon dataset
    //m_selec.SetNNFileName("/home/arnab/Arnab/Work/PhD2021/VLLSingletStudy/MultiClassifier/SignalScore_vllneuron.txt");
  }
  
  if(sample==1){
    chain->Add("/home/riya/Riya/Work/Skimmer/inputs/ZZTo4L.root");
    hstfilename = "outputs/hst_ZZto4L.root";
    sumfilename = "outputs/sum_ZZto4L.txt";
    skimfilename = "outputs/ZZto4L_skimmed.root";
    m_selec.SetData(0); //0 - running over MC, 1 - running over Data
    m_selec.SetYear(2018);
    m_selec.SetMCwt(1);
    m_selec.SetLep(1);
  }
  
  std::cout<<"Output files are "<<hstfilename<<" and "<<sumfilename<<std::endl;
  m_selec.SetHstFileName(hstfilename);
   m_selec.SetSumFileName(sumfilename);
   m_selec.SetSkimFileName(skimfilename);
   m_selec.SetVerbose(0);//set verbosity level for output.
   
   //this calls the Process function for each event in the chain
   //and runs the m_selec code over it.
   chain->Process(&m_selec);
   
}
