import sys
import ROOT

ROOT.gSystem.Load('libRooFit.so')
ROOT.gSystem.Load('libMitExampleFitting.so')

RooFit = ROOT.RooFit
mithep = ROOT.mithep

source = ROOT.TFile.Open('../../macros/Ntuples.root')
tree = source.Get('events')

workspace = ROOT.RooWorkspace('workspace')
argset = workspace.factory('{pt[0., 500.]}')
print argset

dataset = mithep.FittingUtils.createDataSet(tree, argset, "dataset", "T&P dataset")

pdf = workspace.factory('BreitWigner::signal(mass, m0[3.69,3.3,4.], w[0.1,0.,0.4])')

pdf.fitTo(dataset)

frame = mass.frame()
dataset.plotOn(frame)
pdf.plotOn(frame)

frame.Draw()

sys.stdin.readline() # let the script hang around
