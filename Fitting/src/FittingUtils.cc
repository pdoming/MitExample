#include "MitExample/Fitting/interface/FittingUtils.h"
#include "MitExample/DataFormats/interface/TnPEvent.h"

#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooDataSet.h"
#include "TTree.h"
#include "TLorentzVector.h"

RooDataSet*
mithep::FittingUtils::createDataSet(TTree* _source, RooArgSet* _argset, char const* _name/* = "dataset"*/, char const* _title/* = "T&P dataset"*/)
{
  mithep::TnPEvent event;
  event.setAddress(*_source);

  // obtain variables of interest from the arg set
  RooRealVar* pt(dynamic_cast<RooRealVar*>(_argset->find("pt")));
  if (!pt)
    return 0;

  RooDataSet* dataset(new RooDataSet(_name, _title, *_argset));

  long iEntry(0);
  while (_source->GetEntry(iEntry++) > 0) {
    for (unsigned iP(0); iP != event.nPairs; ++iP) {
      auto&& pair(event.at(iP));
      mithep::Tag& tag(pair.first);
      mithep::Probe& probe(pair.second);

      // Apply cuts

      TLorentzVector pTag(tag.px, tag.py, tag.pz, tag.energy);
      TLorentzVector pProbe(probe.px, probe.py, probe.pz, probe.energy);

      // Set variable values

      pt->setVal((pTag + pProbe).Pt());

      dataset->add(*_argset);
    }
  }

  return dataset;
}
