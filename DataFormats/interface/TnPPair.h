#ifndef MITEXAMPLE_DATAFORMATS_TNPPAIR_H
#define MITEXAMPLE_DATAFORMATS_TNPPAIR_H

namespace mithep {

  class TnPEvent;
  class Electron;
  class Photon;

  class TnPPair {
  public:
    TnPPair() {}
    void init(TnPEvent&, unsigned);

    float tagPt() const { return *tagPt_; }
    float tagEta() const { return *tagEta_; }
    float tagPhi() const { return *tagPhi_; }
    float tagPx() const { return *tagPx_; }
    float tagPy() const { return *tagPy_; }
    float tagPz() const { return *tagPz_; }
    float tagEnergy() const { return *tagEnergy_; }

    float probePt() const { return *probePt_; }
    float probeEta() const { return *probeEta_; }
    float probePhi() const { return *probePhi_; }
    float probePx() const { return *probePx_; }
    float probePy() const { return *probePy_; }
    float probePz() const { return *probePz_; }
    float probeEnergy() const { return *probeEnergy_; }

    void set(Electron const&, Photon const&);

  private:
    float* tagPt_;
    float* tagEta_;
    float* tagPhi_;
    float* tagPx_;
    float* tagPy_;
    float* tagPz_;
    float* tagEnergy_;
      
    float* probePt_;
    float* probeEta_;
    float* probePhi_;
    float* probePx_;
    float* probePy_;
    float* probePz_;
    float* probeEnergy_;
  };

}

#endif
