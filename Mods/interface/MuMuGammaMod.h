#ifndef MITEXAMPLE_MODS_MUMUGAMMAMOD_H
#define MITEXAMPLE_MODS_MUMUGAMMAMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h"
#include "MitAna/DataTree/interface/MuonCol.h"
#include "MitAna/DataTree/interface/PhotonCol.h"
#include "MitAna/DataTree/interface/TriggerObjectCol.h"

#include "TLorentzVector.h"
#include "TTree.h"
#include "TString.h"
#include "TObject.h"

namespace mithep {
  
  class MuMuGammaMod : public BaseMod {
  public:
    MuMuGammaMod(char const* name = "MuMuGammaMod", char const* title = "Flat-tree MuMuGamma producer");
    void SetMuonsName(char const* _name) { fMuonsName = _name; }
    void SetMediumPhotonsName(char const* _name) { fMediumPhotonsName = _name; }
    void SetLoosePhotonsName(char const* _name) { fLoosePhotonsName = _name; }
    void SetTriggerObjectsName(char const* _name) { fTriggerObjectsName = _name; }
    void SetTriggerMatchName(char const* _name) { fTriggerMatchName = _name; }

    void SetMinZMassCut(Float_t cut) { MinZMassCut = cut; }
    void SetMaxZMassCut(Float_t cut) { MaxZMassCut = cut; }

  protected:
    void Process() override;
    void SlaveBegin() override;
    void SlaveTerminate() override;
    void SetupMyBranches(TTree&);
    bool MediumPhoton(TLorentzVector muon1Data, TLorentzVector muon2Data, TLorentzVector mediumPhotonData,
                      Float_t MinZMassCut, Float_t MaxZMassCut);
    bool LoosePhoton(TLorentzVector muon1Data, TLorentzVector muon2Data, TLorentzVector loosePhotonData, 
                     Float_t MinZMassCut, Float_t MaxZMassCut);

    TString fMuonsName;
    TString fMediumPhotonsName;
    TString fLoosePhotonsName;
    TString fTriggerObjectsName;
    TString fTriggerMatchName;

    MuonCol const* fMuons;
    PhotonCol const* fMediumPhotons;
    PhotonCol const* fLoosePhotons;
    TriggerObjectCol const* fTriggerObjects;

    TTree* fMuMuGamma;

    static UInt_t const  NMAX = 5;
    Float_t muon1Pt;
    Float_t muon1Eta;
    Float_t muon1Phi;
    Float_t muon1Mass;
    Float_t muon2Pt;
    Float_t muon2Eta;
    Float_t muon2Phi;
    Float_t muon2Mass;
    Float_t mediumPhotonPt;
    Float_t mediumPhotonEta;
    Float_t mediumPhotonPhi;
    Float_t mediumPhotonM;
    Float_t loosePhotonPt;
    Float_t loosePhotonEta;
    Float_t loosePhotonPhi;
    Float_t loosePhotonM;

    // Declaration of TLorentzVectors for photon selection
    TLorentzVector muon1Data;
    TLorentzVector muon2Data;
    TLorentzVector mediumPhotonData;
    TLorentzVector loosePhotonData;

    Float_t MinZMassCut;
    Float_t MaxZMassCut;

    ClassDef(MuMuGammaMod, 0)
  };

}

#endif
