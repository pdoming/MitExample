import ROOT
ROOT.gROOT.SetBatch(True)

ROOT.gSystem.Load('libMitAnaDataTree.so')
ROOT.gSystem.Load('libMitPhysicsMods.so')
ROOT.gSystem.Load('libMitExampleMods.so')

mithep = ROOT.mithep

analysis = mithep.Analysis()
analysis.SetOutputName('ntuples.root')
# analysis.SetKeepHierarchy(False)

analysis.AddFile('/mnt/hadoop/cms/store/user/paus/filefi/032/r12b-dmu-j22-v1/6A568598-3369-E211-AD8E-00259073E4A2.root')
# analysis.SetProcessNEvents(1000)

hltMod = mithep.HLTMod()
#hltMod.SetPrintTable(True)
hltMod.SetBitsName('HLTBits')
hltMod.SetTrigObjsName('DoubleMuTriggerObjects')
hltMod.AddTrigger('HLT_Mu17_v*')
hltMod.AddTrigger('HLT_Mu17_Mu8_v*')
hltMod.AddTrigger('HLT_Mu17_TkMu8_v*')
hltMod.AddTrigger('HLT_Mu22_TkMu8_v*')

# ------------------------------------------
goodPVMod = mithep.GoodPVFilterMod()
goodPVMod.SetMinVertexNTracks(0)
goodPVMod.SetMinNDof(4)
goodPVMod.SetMaxAbsZ(24.0)
goodPVMod.SetMaxRho(2.0)
goodPVMod.SetIsMC(False)
goodPVMod.SetVertexesName('PrimaryVertexes')
# ------------------------------------------

# ------------------------------------------
muonIdMod = mithep.MuonIDMod()
muonIdMod.SetPtMin(10.)
# muonIdMod.SetEtaMax(2.5)
# muonIdMod.SetApplyEcalFiducial(True)
#muonIdMod.SetIDType('NoId')
#muonIdMod.SetIsoType('PFIso')
# muonIdMod.SetApplyConversionFilterType1(False)
# muonIdMod.SetApplyConversionFilterType2(False)
# muonIdMod.SetChargeFilter(False)
#muonIdMod.SetApplyD0Cut(True)
#muonIdMod.SetApplyDZCut(True)
#muonIdMod.SetWhichVertex(-2)
# muonIdMod.SetNExpectedHitsInnerCut(2)
# muonIdMod.SetElectronsFromBranch(True)
muonIdMod.SetInputName('Muons')
muonIdMod.SetOutputName('TightMuons')
# muonIdMod.SetGoodmuonsName('Tightmuons')
# muonIdMod.SetRhoType(mithep.RhoUtilities.CMS_RHO_RHOKT6PFJETS)
# ------------------------------------------

# Other functions - MuonIDMod.h
# SetPrintMVADebugInfo(Bool_t b)
# SetCaloIsoCut(Double_t cut)
# SetClassType(const char *type)
# SetCleanMuonsName(const char *name)
# SetOldMuonsName(const char *n)
# SetOldElectronsName(const char *n)
# SetCleanName(const char *name)
# SetCombIsoCut(Double_t cut)
# SetCombRelativeIsoCut(Double_t cut)
# SetPFIsoCut(Double_t cut)
# SetD0Cut(Double_t cut)
# SetDZCut(Double_t cut)
# SetEtaCut(Double_t cut)
# SetOutputName(const char *name)
# SetTrackIsoCut(Double_t cut)
# SetIntRadius(Double_t dr)
# SetPFNoPileUpName(const char *n)
# SetPFPileUpName(const char *n)

# ------------------------------------------
phoIdMod = mithep.PhotonIDMod()
phoIdMod.SetPtMin(5.0)
phoIdMod.SetOutputName('MediumPhotons')
phoIdMod.SetIDType('EgammaMedium')
phoIdMod.SetIsoType('MITPUCorrected')
phoIdMod.SetApplyElectronVeto(True)
phoIdMod.SetApplyPixelSeed(False)
phoIdMod.SetApplyConversionId(False)
phoIdMod.SetApplyFiduciality(True)
phoIdMod.SetIsData(True)
phoIdMod.SetPhotonsFromBranch(True)
# ------------------------------------------

mumuGammaMod = mithep.MuMuGammaMod('MuMuGammaMod', 'Flat mumuGamma producer')
mumuGammaMod.SetMuonsName('TightMuons')
mumuGammaMod.SetMediumPhotonsName('MediumPhotons')
mumuGammaMod.SetTriggerObjectsName('DoubleMuTriggerObjects')

analysis.AddSuperModule(hltMod)
hltMod.Add(goodPVMod)
goodPVMod.Add(muonIdMod)
muonIdMod.Add(phoIdMod)
phoIdMod.Add(mumuGammaMod)

analysis.Run(False)
