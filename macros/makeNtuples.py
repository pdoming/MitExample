import ROOT
ROOT.gROOT.SetBatch(True)

ROOT.gSystem.Load('libMitAnaDataTree.so')
ROOT.gSystem.Load('libMitPhysicsMods.so')
ROOT.gSystem.Load('libMitExampleMods.so')

mithep = ROOT.mithep

analysis = mithep.Analysis()
analysis.SetOutputName('ntuples.root')

analysis.AddFile(analysis.AddFile('/mnt/hadoop/cms/store/user/paus/filefi/032/r12a-dmu-j22-v1/001AE30A-BA81-E211-BBE7-003048FFD770.root')
analysis.SetProcessNEvents(1000)

hltMod = mithep.HLTMod()
hltMod.SetBitsName('HLTBits')
# Check for naming discrepancy
hltMod.SetTrigObjsName('DoubleMuonTriggerObjects')
hltMod.AddTrigger('HLT_Mu17_v5')

# ------------------------------------------
# I don't think this has to be changed
goodPVMod = mithep.GoodPVFilterMod()
goodPVMod.SetMinVertexNTracks(0)
goodPVMod.SetMinNDof(4)
goodPVMod.SetMaxAbsZ(24.0)
goodPVMod.SetMaxRho(2.0)
goodPVMod.SetIsMC(False)
goodPVMod.SetVertexesName('PrimaryVertexes')
# ------------------------------------------

# ------------------------------------------
# Not sure how/if I should change the inputs in these functions
muonIdMod = mithep.MuonIDMod()
muonIdMod.SetPtMin(30.)
muonIdMod.SetEtaMax(2.5)
muonIdMod.SetApplyEcalFiducial(True)
muonIdMod.SetIDType('CustomTight')
muonIdMod.SetIsoType('PFIso')
muonIdMod.SetApplyConversionFilterType1(False)
muonIdMod.SetApplyConversionFilterType2(False)
muonIdMod.SetChargeFilter(False)
muonIdMod.SetApplyD0Cut(True)
muonIdMod.SetApplyDZCut(True)
muonIdMod.SetWhichVertex(0)
muonIdMod.SetNExpectedHitsInnerCut(2)
muonIdMod.SetmuonsFromBranch(True)
muonIdMod.SetInputName('muons')
muonIdMod.SetGoodmuonsName('Tightmuons')
muonIdMod.SetRhoType(mithep.RhoUtilities.CMS_RHO_RHOKT6PFJETS)
# ------------------------------------------

# ------------------------------------------
# I don't think this has to be changed.
phoIdMod = mithep.PhotonIDMod()
phoIdMod.SetPtMin(10.0)
phoIdMod.SetOutputName('MediumPhotonsNoEVeto')
phoIdMod.SetIDType('EgammaMedium')
phoIdMod.SetIsoType('MITPUCorrected')
phoIdMod.SetApplyElectronVeto(False)
phoIdMod.SetApplyPixelSeed(False)
phoIdMod.SetApplyConversionId(False)
phoIdMod.SetApplyFiduciality(True)
phoIdMod.SetIsData(True)
phoIdMod.SetPhotonsFromBranch(True)
# ------------------------------------------

ntuplesMod = mithep.NtuplesMod('NtuplesMod', 'Flat ntuples producer')
# Check for naming discrepancies
ntuplesMod.SetTagMuonsName('TightMuons')
ntuplesMod.SetProbePhotonsName('MediumPhotonsNoEVeto')
# Check for naming discrepancies
ntuplesMod.SetTriggerObjectsName('SingleMuonTriggerObjects')
ntuplesMod.SetTriggerMatchName('hltEle27WP80TrackIsoFilter')

analysis.AddSuperModule(hltMod)
hltMod.Add(goodPVMod)
goodPVMod.Add(muonIdMod)
muonIdMod.Add(phoIdMod)
phoIdMod.Add(ntuplesMod)

analysis.Run(False)
