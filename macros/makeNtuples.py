import ROOT
ROOT.gROOT.SetBatch(True)

ROOT.gSystem.Load('libMitAnaDataTree.so')
ROOT.gSystem.Load('libMitExampleMods.so')

mithep = ROOT.mithep

anlaysis = mithep.Analysis()
analysis.SetOutputName('ntuples.root')

analysis.AddFile('bambu-output-file-ttmini_000.root')
analysis.SetProcessNEvents(1000)

goodPVMod = mithep.GoodPVFilterMod()
goodPVMod.SetMinVertexNTracks(0)
goodPVMod.SetMinNDof(4)
goodPVMod.SetMaxAbsZ(24.0)
goodPVMod.SetMaxRho(2.0)
goodPVMod.SetIsMC(True)
goodPVMod.SetVertexesName("PrimaryVertexes")

eleIdMod = mithep.ElectronIDMod()
eleIdMod.SetPtMin(30.)
eleIdMod.SetEtaMax(2.5)
eleIdMod.SetApplyEcalFiducial(True)
eleIdMod.SetIDType("CustomIdTight")
eleIdMod.SetIsoType("PFIso")
eleIdMod.SetApplyConversionFilterType1(False)
eleIdMod.SetApplyConversionFilterType2(False)
eleIdMod.SetChargeFilter(False)
eleIdMod.SetApplyD0Cut(True)
eleIdMod.SetApplyDZCut(True)
eleIdMod.SetWhichVertex(0)
eleIdMod.SetNExpectedHitsInnerCut(2)
eleIdMod.SetElectronsFromBranch(True)
eleIdMod.SetInputName("Electrons")
eleIdMod.SetGoodElectronsName("TightElectrons")
eleIdMod.SetRhoType(mithep.RhoUtilities.CMS_RHO_RHOKT6PFJETS)

phoIdMod = mithep.PhotonIDMod()
phoIdMod.SetPtMin(10.0)
phoIdMod.SetOutputName("MediumPhotonsNoEVeto")
phoIdMod.SetIDType("EgammaMedium")
phoIdMod.SetIsoType("MITPUCorrected")
phoIdMod.SetApplyElectronVeto(False)
phoIdMod.SetApplyPixelSeed(False)
phoIdMod.SetApplyConversionId(False)
phoIdMod.SetApplyFiduciality(True)
phoIdMod.SetIsData(False)
phoIdMod.SetPhotonsFromBranch(True)

ntuplesMod = mithep.NtuplesMod('NtuplesMod', 'Flat ntuples producer')
ntuplesMod.SetTagElectronsName("TightElectrons")
ntuplesMod.SetProbePhotonsName("MediumPhotonsNoEVeto")

analysis.AddSuperModule(goodPVMod)
goodPVMod.Add(eleIdMod)
eleIdMod.Add(phoIdMod)
phoIdMod.Add(ntuplesMod)

analysis.Run(False)
