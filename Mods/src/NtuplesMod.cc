#include "MitExample/Mods/interface/NtuplesMod.h"

#include "MitAna/DataTree/interface/Names.h"

#include "TVector2.h"

#include <vector>
#include <cstring>

ClassImp(mithep::NtuplesMod)

namespace mithep {

  NtuplesMod::NtuplesMod(char const* _name/* = "NtuplesMod"*/, char const* _title/* = "Flat-tree ntuples producer"*/) :
    BaseMod(_name, _title),
    fTagElectronsName("TagElectrons"),
    fProbePhotonsName("ProbePhotons"),
    fTriggerObjectsName(Names::gkHltObjBrn),
    fTagElectrons(0),
    fProbePhotons(0),
    fEvent(),
    fNtuples(0)
  {
  }

  void
  NtuplesMod::Process()
  {
    LoadEventObject(fTagElectronsName, fTagElectrons);
    LoadEventObject(fProbePhotonsName, fProbePhotons);

    if (!fTagElectrons || !fProbePhotons) {
      std::cerr << "Could not find electrons in the event." << std::endl;
      return;
    }

    std::vector<TriggerObject const*> singleEle;
    for (unsigned iO(0); iO != fTriggerObjects->GetEntries(); ++iO) {
      TriggerObject const& to(*fTriggerObjects->At(iO));

      if (std::strcmp(to.ModuleName(), "hltL1EG25Ele27WP85GsfTrackIsoFilter") == 0)
        singleEle.push_back(&to);
    }

    if (singleEle.size() == 0)
      return;

    std::vector<Electron const*> tags;
    for (unsigned iE(0); iE != fTagElectrons->GetEntries(); ++iE) {
      Electron const& inEle(*fTagElectrons->At(iE));

      unsigned iT(0);
      for (; iT != singleEle.size(); ++iT) {
        double dEta(singleEle[iT]->Eta() - inEle.Eta());
        double dPhi(TVector2::Phi_mpi_pi(singleEle[iT]->Phi() - inEle.Phi()));

        if (dEta * dEta + dPhi * dPhi < 0.15 * 0.15)
          break;
      }
      tags.push_back(&inEle);
    }

    std::vector<Photon const*> probes;
    for (unsigned iP(0); iP != fProbePhotons->GetEntries(); ++iP) {
      Photon const& inPh(*fProbePhotons->At(iP));

      probes.push_back(&inPh);
    }

    for (Electron const* tag : tags) {
      for (Photon const* probe : probes) {
        // candidates overlap in supercluster -> a same EG object
        if (tag->SCluster() == probe->SCluster())
          continue;

        fEvent.pairs[fEvent.nPairs].set(*tag, *probe);
        ++fEvent.nPairs;
      }
    }

    fNtuples->Fill();
  }

  void
  NtuplesMod::SlaveBegin()
  {
    fNtuples = new TTree("events", "Double Electron events");
    fEvent.bookBranches(*fNtuples);

    AddOutput(fNtuples);
  }

  void
  NtuplesMod::SlaveTerminate()
  {
  }

}
