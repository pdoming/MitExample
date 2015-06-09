#include "MitExample/Mods/interface/NtuplesMod.h"

#include "MitAna/DataTree/interface/Names.h"

#include "TVector2.h"

#include <vector>
#include <cstring>

ClassImp(mithep::NtuplesMod)

mithep::NtuplesMod::NtuplesMod(char const* _name/* = "mithep::NtuplesMod"*/, char const* _title/* = "Flat-tree ntuples producer"*/) :
BaseMod(_name, _title),
  fTagElectronsName("TagElectrons"),
  fProbePhotonsName("ProbePhotons"),
  fTriggerObjectsName(mithep::Names::gkHltObjBrn),
  fTriggerMatchName(""),
  fTagElectrons(0),
  fProbePhotons(0),
  fEvent(),
  fNtuples(0)
{
}

void
mithep::NtuplesMod::Process()
{
  LoadEventObject(fTagElectronsName, fTagElectrons);
  LoadEventObject(fProbePhotonsName, fProbePhotons);

  if (!fTagElectrons || !fProbePhotons) {
    std::cerr << "Could not find electrons in the event." << std::endl;
    return;
  }

  bool doTriggerMatch(fTriggerMatchName.Length() != 0);

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

  std::vector<Electron const*> tags;
  for (unsigned iE(0); iE != fTagElectrons->GetEntries(); ++iE) {
    Electron const& inEle(*fTagElectrons->At(iE));

    if (doTriggerMatch) {
      unsigned iT(0);
      for (; iT != matchObjects.size(); ++iT) {
        double dEta(matchObjects[iT]->Eta() - inEle.Eta());
        double dPhi(TVector2::Phi_mpi_pi(matchObjects[iT]->Phi() - inEle.Phi()));

        if (dEta * dEta + dPhi * dPhi < 0.15 * 0.15)
          break;
      }
      if (iT == matchObjects.size())
        continue;
    }

    // apply more cuts to tag

    tags.push_back(&inEle);
  }

  std::vector<Photon const*> probes;
  for (unsigned iP(0); iP != fProbePhotons->GetEntries(); ++iP) {
    Photon const& inPh(*fProbePhotons->At(iP));

    // apply some additional cuts to probe

    probes.push_back(&inPh);
  }

  fEvent.clear();

  for (Electron const* tag : tags) {
    for (Photon const* probe : probes) {
      // candidates overlap in supercluster -> a same EG object
      if (tag->SCluster() == probe->SCluster())
        continue;

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
  fNtuples = new TTree("events", "Double Electron events");
  fEvent.bookBranches(*fNtuples);

  AddOutput(fNtuples);
}

void
mithep::NtuplesMod::SlaveTerminate()
{
}
