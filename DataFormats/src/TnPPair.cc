#include "MitExample/DataFormats/interface/TnPPair.h"

#include "MitExample/DataFormats/interface/TnPEvent.h"
#include "MitAna/DataTree/interface/Photon.h"
#include "MitAna/DataTree/interface/Electron.h"

namespace mithep {

  void
  TnPPair::init(TnPEvent& _evt, unsigned _iP)
  {
    tagPt_ = &_evt.tagPt[_iP];
    tagEta_ = &_evt.tagEta[_iP];
    tagPhi_ = &_evt.tagPhi[_iP];
    tagPx_ = &_evt.tagPx[_iP];
    tagPy_ = &_evt.tagPy[_iP];
    tagPz_ = &_evt.tagPz[_iP];
    tagEnergy_ = &_evt.tagEnergy[_iP];

    probePt_ = &_evt.probePt[_iP];
    probeEta_ = &_evt.probeEta[_iP];
    probePhi_ = &_evt.probePhi[_iP];
    probePx_ = &_evt.probePx[_iP];
    probePy_ = &_evt.probePy[_iP];
    probePz_ = &_evt.probePz[_iP];
    probeEnergy_ = &_evt.probeEnergy[_iP];
  }

  void
  TnPPair::set(Electron const& _tag, Photon const& _probe)
  {
    *tagPt_ = _tag.Pt();
    *tagEta_ = _tag.Eta();
    *tagPhi_ = _tag.Phi();
    *tagPx_ = _tag.Px();
    *tagPy_ = _tag.Py();
    *tagPz_ = _tag.Pz();
    *tagEnergy_ = _tag.E();

    *probePt_ = _probe.Pt();
    *probeEta_ = _probe.Eta();
    *probePhi_ = _probe.Phi();
    *probePx_ = _probe.Px();
    *probePy_ = _probe.Py();
    *probePz_ = _probe.Pz();
    *probeEnergy_ = _probe.E();
  }

}
