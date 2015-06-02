#include "MitExample/DataFormats/interface/TnPEvent.h"

#include "TTree.h"

namespace mithep {

  TnPEvent::TnPEvent() :
    nPairs(0)
  {
    for (unsigned iP(0); iP != NMAX; ++iP)
      pairs[iP].init(*this, iP);
  }

  void
  TnPEvent::bookBranches(TTree& _tree)
  {
    _tree.Branch("nPairs", &nPairs, "nPairs/i");

    _tree.Branch("tag.pt", tagPt, "pt[nPairs]/F");
    _tree.Branch("tag.eta", tagEta, "eta[nPairs]/F");
    _tree.Branch("tag.phi", tagPhi, "phi[nPairs]/F");
    _tree.Branch("tag.px", tagPx, "px[nPairs]/F");
    _tree.Branch("tag.py", tagPy, "py[nPairs]/F");
    _tree.Branch("tag.pz", tagPz, "pz[nPairs]/F");
    _tree.Branch("tag.energy", tagEnergy, "energy[nPairs]/F");

    _tree.Branch("probe.pt", probePt, "pt[nPairs]/F");
    _tree.Branch("probe.eta", probeEta, "eta[nPairs]/F");
    _tree.Branch("probe.phi", probePhi, "phi[nPairs]/F");
    _tree.Branch("probe.px", probePx, "px[nPairs]/F");
    _tree.Branch("probe.py", probePy, "py[nPairs]/F");
    _tree.Branch("probe.pz", probePz, "pz[nPairs]/F");
    _tree.Branch("probe.energy", probeEnergy, "energy[nPairs]/F");
  }

}
