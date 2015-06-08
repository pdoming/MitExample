import ROOT
ROOT.gROOT.SetBatch(True)

ROOT.gSystem.Load('libMitAnaDataTree.so')
ROOT.gSystem.Load('libMitPhysicsMods.so')
ROOT.gSystem.Load('libMitExampleMods.so')

mithep = ROOT.mithep

analysis = mithep.Analysis()
analysis.SetOutputName('ntuples.root')

analysis.AddFile('/mnt/hadoop/cms/store/user/paus/filefi/032/r12a-dmu-j22-v1/001AE30A-BA81-E211-BBE7-003048FFD770.root')
analysis.SetProcessNEvents(1000)

hltMod = mithep.HLTMod()
hltMod.SetBitsName('HLTBits')
hltMod.SetTrigObjsName('DoubleMuTriggerObjects')
hltMod.AddTrigger('HLT_Mu22_TkMu8_v*')            # http://j2eeps.cern.ch/cms-project-confdb-hltdev/browser/ click cdaq/physics/Run2012/

goodPVMod = mithep.GoodPVFilterMod()
goodPVMod.SetMinVertexNTracks(0)
goodPVMod.SetMinNDof(4)
goodPVMod.SetMaxAbsZ(24.0)
goodPVMod.SetMaxRho(2.0)
goodPVMod.SetIsMC(False)
goodPVMod.SetVertexesName('PrimaryVertexes')

muIdMod = mithep.MuonIDMod()
muIdMod.SetPtMin(10.0 )
muIdMod.SetInputName('Muons')
muIdMod.SetOutputName('TightMuons')

phoIdMod = mithep.PhotonIDMod()
phoIdMod.SetPtMin(10.0)
phoIdMod.SetOutputName('MediumPhotons')
phoIdMod.SetIDType('EgammaMedium')
phoIdMod.SetIsoType('MITPUCorrected')
phoIdMod.SetApplyElectronVeto(True)
phoIdMod.SetApplyPixelSeed(False)
phoIdMod.SetApplyConversionId(False)
phoIdMod.SetApplyFiduciality(True)
phoIdMod.SetIsData(True)
phoIdMod.SetPhotonsFromBranch(True)

ntuplesMod = mithep.NtuplesMod('NtuplesMod', 'Flat ntuples producer')
ntuplesMod.SetTagMuonsName('TightMuons')         # ?
ntuplesMod.SetProbePhotonsName('MediumPhotons')
ntuplesMod.SetTriggerObjectsName('DoubleMuTriggerObjects')

analysis.AddSuperModule(hltMod)
hltMod.Add(goodPVMod)
goodPVMod.Add(muIdMod)
muIdMod.Add(phoIdMod)
phoIdMod.Add(ntuplesMod)

analysis.Run(False)
