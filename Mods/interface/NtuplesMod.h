#ifndef MITEXAMPLE_MODS_NTUPLESMOD_H
#define MITEXAMPLE_MODS_NTUPLESMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h"
#include "MitAna/DataTree/interface/PhotonCol.h"
#include "MitAna/DataTree/interface/TriggerObjectCol.h"
#include "MitAna/DataTree/interface/MuonCol.h"
#include "MitExample/DataFormats/interface/TnPEvent.h"

#include "TTree.h"
#include "TString.h"

namespace mithep {
  
  class NtuplesMod : public BaseMod {
  public:
    NtuplesMod(char const* name = "NtuplesMod", char const* title = "Flat-tree ntuples producer");
    void SetTagMuonsName(char const* _name) { fTagMuonsName = _name; }
    void SetProbePhotonsName(char const* _name) { fProbePhotonsName = _name; }
    void SetTriggerObjectsName(char const* _name) { fTriggerObjectsName = _name; }
    void SetTriggerMatchName(char const* _name) { fTriggerMatchName = _name; }

  protected:
    void Process() override;
    void SlaveBegin() override;
    void SlaveTerminate() override;

    TString fTagMuonsName;
    TString fProbePhotonsName;
    TString fTriggerObjectsName;
    TString fTriggerMatchName;

    MuonCol const* fTagMuons;
    PhotonCol const* fProbePhotons;
    TriggerObjectCol const* fTriggerObjects;

    TnPEvent fEvent;
    TTree* fNtuples;

    ClassDef(NtuplesMod, 0)
  };

}

#endif
