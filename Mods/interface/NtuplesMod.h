#ifndef MITEXAMPLE_MODS_NTUPLESMOD_H
#define MITEXAMPLE_MODS_NTUPLESMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h"
#include "MitAna/DataTree/interface/MuonCol.h"

#include "TTree.h"
#include "TString.h"

namespace mithep {
  
  class NtuplesMod : public BaseMod {
  public:
    NtuplesMod(char const* name = "NtuplesMod", char const* title = "Flat-tree ntuples producer");

    static unsigned const NMAX = 64;

    class Event;

    class Muon {
    public:
      Muon() {}
      void init(Event&, unsigned);

      float* pt;
      float* eta;
      float* phi;
      float* px;
      float* py;
      float* pz;
      float* energy;
      short* charge;
      bool* isTight;
      bool* isSoft;
    };

    class Event {
    public:
      Event();

      unsigned muN;
      Muon muons[NMAX];

      float muPt[NMAX];
      float muEta[NMAX];
      float muPhi[NMAX];
      float muPx[NMAX];
      float muPy[NMAX];
      float muPz[NMAX];
      float muEnergy[NMAX];
      short muCharge[NMAX];
      bool muIsTight[NMAX];
      bool muIsSoft[NMAX];
    };

  protected:
    void Process() override;
    void SlaveBegin() override;
    void SlaveTerminate() override;

    TString fTightMuonsName;
    TString fSoftMuonsName;

    mithep::MuonCol const* fTightMuons;
    mithep::MuonCol const* fSoftMuons;

    Event fEvent;
    TTree* fNtuples;

    ClassDef(NtuplesMod, 0)
  };

}

#endif
