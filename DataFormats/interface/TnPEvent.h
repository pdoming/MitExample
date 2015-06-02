#ifndef MITEXAMPLE_DATAFORMATS_TNPEVENT_H
#define MITEXAMPLE_DATAFORMATS_TNPEVENT_H

#include "MitExample/DataFormats/interface/TnPPair.h"

class TTree;

namespace mithep {

  class TnPEvent {
  public:
    static unsigned const NMAX = 64;
    
    TnPEvent();
    void bookBranches(TTree&);

    unsigned nPairs;
    TnPPair pairs[NMAX];

    float tagPt[NMAX];
    float tagEta[NMAX];
    float tagPhi[NMAX];
    float tagPx[NMAX];
    float tagPy[NMAX];
    float tagPz[NMAX];
    float tagEnergy[NMAX];
      
    float probePt[NMAX];
    float probeEta[NMAX];
    float probePhi[NMAX];
    float probePx[NMAX];
    float probePy[NMAX];
    float probePz[NMAX];
    float probeEnergy[NMAX];
  };

}

#endif
