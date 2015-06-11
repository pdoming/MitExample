#include "MitExample/Mods/interface/MuMuGammaMod.h"
#include "MitAna/DataTree/interface/Names.h"
#include "TVector2.h"
#include "TLorentzVector.h"
#include "TMath.h"

#include <vector>
#include <cstring>

ClassImp(mithep::MuMuGammaMod)

mithep::MuMuGammaMod::MuMuGammaMod(char const* _name/* = "mithep::MuMuGammaMod"*/, char const* _title/* = "Flat-tree MuMuGamma producer"*/) :
  BaseMod(_name, _title),
  fMuonsName("Muons"),
  fMediumPhotonsName("MediumPhotons"),
  fLoosePhotonsName(mithep::Names::gkPhotonBrn),
  fTriggerObjectsName(mithep::Names::gkHltObjBrn),
  fTriggerMatchName(""),
  fMuons(0),
  fMediumPhotons(0),
  fLoosePhotons(0),
  fMuMuGamma(0),
  muon1Pt(-1),
  muon1Eta(0),
  muon1Phi(0),
  muon1Mass(0),
  muon2Pt(-1),
  muon2Eta(0),
  muon2Phi(0),
  muon2Mass(0),
  mediumPhotonPt(-1),
  mediumPhotonEta(0),
  mediumPhotonPhi(0),
  loosePhotonPt(-1),
  loosePhotonEta(0),
  loosePhotonPhi(0),
  MinZMassCut(0),
  MaxZMassCut(180)

{
}

void
mithep::MuMuGammaMod::Process()
{
  LoadEventObject(fMuonsName, fMuons);
  LoadEventObject(fMediumPhotonsName, fMediumPhotons);
  LoadEventObject(fLoosePhotonsName, fLoosePhotons);

  if (!fMuons || !fMediumPhotons || !fLoosePhotons) {
    std::cerr << "Could not find muons in the event." << std::endl;
    return;
  }

  muon1Pt = -1;
  muon1Eta = 0;
  muon1Phi = 0;
  muon1Mass = 0;
  muon2Pt = -1;
  muon2Eta = 0;
  muon2Phi = 0;
  muon2Mass = 0;
  mediumPhotonPt = -1;
  mediumPhotonEta = 0;
  mediumPhotonPhi = 0;
  loosePhotonPt = -1;
  loosePhotonEta = 0;
  loosePhotonPhi = 0;
  MinZMassCut = 0;
  MaxZMassCut = 0;

  std::vector<Muon const*> muons;
  for (unsigned iE(0); iE != fMuons->GetEntries(); ++iE) {
    Muon const& inMu(*fMuons->At(iE));

    muons.push_back(&inMu);
  }

  std::vector<Photon const*> mediumPhotons;
  for (unsigned iP(0); iP != fMediumPhotons->GetEntries(); ++iP) {
    Photon const& inPh(*fMediumPhotons->At(iP));

    // apply some additional cuts to mediumPhoton
    if ((MediumPhoton(muon1Data, muon2Data, mediumPhotonData, MinZMassCut, MaxZMassCut) == true)
        && (LoosePhoton(muon1Data, muon2Data, loosePhotonData, MinZMassCut, MaxZMassCut) != true)) {
      mediumPhotons.push_back(&inPh);
    }
    else if (mediumPhotonData.Pt() > loosePhotonData.Pt()) {
      mediumPhotons.push_back(&inPh);
    }

  }

  std::vector<Photon const*> loosePhotons;
  for (unsigned iP(0); iP != fLoosePhotons->GetEntries(); ++iP) {
    Photon const& inPh(*fLoosePhotons->At(iP));

    // apply some additional cuts to loosePhoton
    if ((MediumPhoton(muon1Data, muon2Data, mediumPhotonData, MinZMassCut, MaxZMassCut) == true)
        && (LoosePhoton(muon1Data, muon2Data, loosePhotonData, MinZMassCut, MaxZMassCut) != true)) {
      loosePhotons.push_back(&inPh);
    }
    else if (loosePhotonData.Pt() > mediumPhotonData.Pt()) {
      loosePhotons.push_back(&inPh);
    }

  }
  
  for (Muon const* muon : muons) {
    if (muon->Pt() > muon1Pt){
      muon2Pt = muon1Pt;
      muon2Eta = muon1Eta;
      muon2Phi = muon1Phi;
      muon2Mass = muon1Mass;
      muon1Pt = muon->Pt();
      muon1Eta = muon->Eta();
      muon1Phi = muon->Phi();
      muon1Mass = muon->Mass();
      muon1Data.SetPtEtaPhiM(muon1Pt, muon1Eta, muon1Phi, muon1Mass);
    } 

    else if (muon->Pt() > muon2Pt){
      muon2Pt = muon->Pt();
      muon2Eta = muon->Eta();
      muon2Phi = muon->Phi();
      muon2Mass = muon->Mass();
      muon2Data.SetPtEtaPhiM(muon2Pt, muon2Eta, muon2Phi, muon2Mass);
    }

  }

  for (Photon const* mediumPhoton : mediumPhotons) {
    if (mediumPhoton->Pt() > mediumPhotonPt){
      mediumPhotonPt = mediumPhoton->Pt();
      mediumPhotonEta = mediumPhoton->Eta();
      mediumPhotonPhi = mediumPhoton->Phi();
      mediumPhotonM = 0.;
      mediumPhotonData.SetPtEtaPhiM(mediumPhotonPt, mediumPhotonEta, mediumPhotonPhi, mediumPhotonM);
    }
  }

  for (Photon const* loosePhoton : loosePhotons) {
    if (loosePhoton->Pt() > loosePhotonPt){
      loosePhotonPt = loosePhoton->Pt();
      loosePhotonEta = loosePhoton->Eta();
      loosePhotonPhi = loosePhoton->Phi();
      loosePhotonM = 0.;
      loosePhotonData.SetPtEtaPhiM(loosePhotonPt, loosePhotonEta, loosePhotonPhi, loosePhotonM);
    }
  }

  if (muon2Pt > -1)
    fMuMuGamma->Fill();

}


bool
mithep::MuMuGammaMod::MediumPhoton(TLorentzVector muon1Data, TLorentzVector muon2Data, TLorentzVector mediumPhotonData,
                                   Float_t MinZMassCut, Float_t MaxZMassCut) {
  TLorentzVector v1 = muon1Data+muon2Data+mediumPhotonData;
  return (MinZMassCut < v1.M() < MaxZMassCut);
}
bool
mithep::MuMuGammaMod::LoosePhoton(TLorentzVector muon1Data, TLorentzVector muon2Data, TLorentzVector loosePhotonData,
                                  Float_t MinZMassCut, Float_t MaxZMassCut) {
  TLorentzVector v1 = muon1Data+muon2Data+loosePhotonData;
  return (MinZMassCut < v1.M() < MaxZMassCut);
}


void
mithep::MuMuGammaMod::SetupMyBranches(TTree& _tree)
{
  _tree.Branch("muon1Pt",&muon1Pt,"muon1Pt/F");
  _tree.Branch("muon1Eta",&muon1Eta,"muon1Eta/F");
  _tree.Branch("muon1Phi",&muon1Phi,"muon1Phi/F");
  _tree.Branch("muon1Mass",&muon1Mass,"muon1Mass/F");
  _tree.Branch("muon2Pt",&muon2Pt,"muon2Pt/F");
  _tree.Branch("muon2Eta",&muon2Eta,"muon2Eta/F");
  _tree.Branch("muon2Phi",&muon2Phi,"muon2Phi/F");
  _tree.Branch("muon2Mass",&muon2Mass,"muon2Mass/F");
  _tree.Branch("mediumPhotonPt",&mediumPhotonPt,"mediumPhotonPt/F");
  _tree.Branch("mediumPhotonEta",&mediumPhotonEta,"mediumPhotonEta/F");
  _tree.Branch("mediumPhotonPhi",&mediumPhotonPhi,"mediumPhotonPhi/F");
  _tree.Branch("loosePhotonPt",&loosePhotonPt,"loosePhotonPt/F");
  _tree.Branch("loosePhotonEta",&loosePhotonEta,"loosePhotonEta/F");
  _tree.Branch("loosePhotonPhi",&loosePhotonPhi,"loosePhotonPhi/F");
}

void
mithep::MuMuGammaMod::SlaveBegin()
{
  ReqEventObject(fLoosePhotonsName, fLoosePhotons, kTRUE);
  fMuMuGamma = new TTree("events", "Double Muon events");
  SetupMyBranches(*fMuMuGamma);
  // fEvent.bookBranches(*fMuMuGamma);

  AddOutput(fMuMuGamma);
}

void
mithep::MuMuGammaMod::SlaveTerminate()
{
}


