#include "MitExample/Mods/interface/NtuplesMod.h"

#include "MitAna/DataTree/interface/Names.h"

#include "TVector2.h"

#include <vector>
#include <cstring>

ClassImp(mithep::NtuplesMod)

mithep::NtuplesMod::NtuplesMod(char const* _name/* = "mithep::NtuplesMod"*/, char const* _title/* = "Flat-tree ntuples producer"*/) :
  BaseMod(_name, _title),
  fTagMuonsName("TagMuons"),
  fProbePhotonsName("ProbePhotons"),
  fTriggerObjectsName(mithep::Names::gkHltObjBrn),
  fTriggerMatchName(""),
  fTagMuons(0),
  fProbePhotons(0),
  fEvent(),
  fNtuples(0)
{
}

void
mithep::NtuplesMod::Process()
{
  LoadEventObject(fTagMuonsName, fTagMuons);
  LoadEventObject(fProbePhotonsName, fProbePhotons);

  if (!fTagMuons || !fProbePhotons) {
    std::cerr << "Could not find muons in the event." << std::endl;
    return;
  }

  std::vector<TriggerObject const*> matchObjects;

  if (doTriggerMatch) {
    LoadEventObject(fTriggerObjectsName, fTriggerObjects);

    if (!fTriggerObjects) {
      std::cerr << "Could not find trigger objects in the event." << std::endl;
      return;
    } 

    for (unsigned iO(0); iO != fTriggerObjects->GetEntries(); ++iO) {
      TriggerObject const& to(*fTriggerObjects->At(iO));

      if (std::strcmp(to.ModuleName(), fTriggerMatchName) == 0)
        matchObjects.push_back(&to);
    }

    if (matchObjects.size() == 0)
      return;
  }

  std::vector<Muon const*> tags;
  for (unsigned iE(0); iE != fTagMuons->GetEntries(); ++iE) {
    Muon const& inMu(*fTagMuons->At(iE));

    if (doTriggerMatch) {
      unsigned iT(0);
      for (; iT != matchObjects.size(); ++iT) {
        double dEta(matchObjects[iT]->Eta() - inMu.Eta());
        double dPhi(TVector2::Phi_mpi_pi(matchObjects[iT]->Phi() - inMu.Phi()));

        if (dEta * dEta + dPhi * dPhi < 0.15 * 0.15)
          break;
      }
      if (iT == matchObjects.size())
        continue;
    }

    // apply more cuts to tag

    tags.push_back(&inMu);
  }

  std::vector<Photon const*> probes;
  for (unsigned iP(0); iP != fProbePhotons->GetEntries(); ++iP) {
    Photon const& inPh(*fProbePhotons->At(iP));

    // apply some additional cuts to probe

    probes.push_back(&inPh);
  }

  fEvent.clear();

  for (Muon const* tag : tags) {
    for (Photon const* probe : probes) {
      // candidates overlap in supercluster -> a same EG object
      // if (tag->SCluster() == probe->SCluster())    // No such thing as a muon supercluster
      //   continue;

      auto&& pair(fEvent.addNew());

      pair.first.pt = tag->Pt();
      pair.first.eta = tag->Eta();
      pair.first.phi = tag->Phi();
      pair.first.px = tag->Px();
      pair.first.py = tag->Py();
      pair.first.pz = tag->Pz();
      pair.first.energy = tag->E();

      pair.second.pt = probe->Pt();
      pair.second.eta = probe->Eta();
      pair.second.phi = probe->Phi();
      pair.second.px = probe->Px();
      pair.second.py = probe->Py();
      pair.second.pz = probe->Pz();
      pair.second.energy = probe->E();
    }
  }

  fNtuples->Fill();
}

void
mithep::NtuplesMod::SlaveBegin()
{
  fNtuples = new TTree("events", "Double Muon events");
  fEvent.bookBranches(*fNtuples);

  AddOutput(fNtuples);
}

void
mithep::NtuplesMod::SlaveTerminate()
{
}


