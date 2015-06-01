#include "MitExample/Mods/interface/NtuplesMod.h"

#include <vector>

ClassImp(mithep::NtuplesMod)

namespace mithep {

  void
  NtuplesMod::Muon::init(NtuplesMod::Event& _evt, unsigned _iM)
  {
    pt = &_evt.muPt[_iM];
    eta = &_evt.muEta[_iM];
    phi = &_evt.muPhi[_iM];
    px = &_evt.muPx[_iM];
    py = &_evt.muPy[_iM];
    pz = &_evt.muPz[_iM];
    charge = &_evt.muCharge[_iM];
    energy = &_evt.muEnergy[_iM];
    isTight = &_evt.muIsTight[_iM];
    isSoft = &_evt.muIsSoft[_iM];
  }

  NtuplesMod::Event::Event() :
    muN(0)
  {
    for (unsigned iM(0); iM != NMAX; ++iM)
      muons[iM].init(*this, iM);
  }

  NtuplesMod::NtuplesMod(char const* _name/* = "NtuplesMod"*/, char const* _title/* = "Flat-tree ntuples producer"*/) :
    BaseMod(_name, _title),
    fTightMuonsName("TightMuons"),
    fSoftMuonsName("SoftMuons"),
    fTightMuons(0),
    fSoftMuons(0),
    fEvent(),
    fNtuples(0)
  {
  }

  void
  NtuplesMod::Process()
  {
    fTightMuons = GetObjThisEvt<MuonCol>(fTightMuonsName);
    fSoftMuons = GetObjThisEvt<MuonCol>(fSoftMuonsName);

    if (!fTightMuons || !fSoftMuons) {
      std::cerr << "Could not find muons in the event." << std::endl;
      return;
    }

    fEvent.muN = 0;

    for (unsigned iM(0); iM != fTightMuons->GetEntries(); ++iM) {
      mithep::Muon const& inMu(*fTightMuons->At(iM));
      NtuplesMod::Muon& outMu(fEvent.muons[fEvent.muN]);

      *outMu.pt = inMu.Pt();
      *outMu.eta = inMu.Eta();
      *outMu.phi = inMu.Phi();
      *outMu.px = inMu.Px();
      *outMu.py = inMu.Py();
      *outMu.pz = inMu.Pz();
      *outMu.energy = inMu.E();
      *outMu.charge = int(inMu.Charge());
      *outMu.isTight = true;
      *outMu.isSoft = fSoftMuons->HasObject(&inMu);

      ++fEvent.muN;
    }

    for (unsigned iM(0); iM != fSoftMuons->GetEntries(); ++iM) {
      mithep::Muon const& inMu(*fSoftMuons->At(iM));
      inMu.Mom();
      if (fTightMuons->HasObject(&inMu))
        continue;
      NtuplesMod::Muon& outMu(fEvent.muons[fEvent.muN]);

      *outMu.pt = inMu.Pt();
      *outMu.eta = inMu.Eta();
      *outMu.phi = inMu.Phi();
      *outMu.px = inMu.Px();
      *outMu.py = inMu.Py();
      *outMu.pz = inMu.Pz();
      *outMu.energy = inMu.E();
      *outMu.charge = int(inMu.Charge());
      *outMu.isTight = false;
      *outMu.isSoft = true;

      ++fEvent.muN;
    }

    fNtuples->Fill();
  }

  void
  NtuplesMod::SlaveBegin()
  {
    fNtuples = new TTree("events", "Double Muon events");
    fNtuples->Branch("muon.n", &fEvent.muN, "n/i");
    fNtuples->Branch("muon.pt", fEvent.muPt, "pt[muon.n]/F");
    fNtuples->Branch("muon.eta", fEvent.muEta, "eta[muon.n]/F");
    fNtuples->Branch("muon.phi", fEvent.muPhi, "phi[muon.n]/F");
    fNtuples->Branch("muon.px", fEvent.muPx, "px[muon.n]/F");
    fNtuples->Branch("muon.py", fEvent.muPy, "py[muon.n]/F");
    fNtuples->Branch("muon.pz", fEvent.muPz, "pz[muon.n]/F");
    fNtuples->Branch("muon.energy", fEvent.muEnergy, "energy[muon.n]/F");
    fNtuples->Branch("muon.charge", fEvent.muCharge, "charge[muon.n]/S");
    fNtuples->Branch("muon.isTight", fEvent.muIsTight, "isTight[muon.n]/O");
    fNtuples->Branch("muon.isSoft", fEvent.muIsSoft, "isSoft[muon.n]/O");

    AddOutput(fNtuples);
  }

  void
  NtuplesMod::SlaveTerminate()
  {
  }

}
