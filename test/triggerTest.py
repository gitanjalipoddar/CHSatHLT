# import ROOT in batch mode
import sys
oldargv = sys.argv[:]
sys.argv = [ '-b-' ]
import ROOT
ROOT.gROOT.SetBatch(True)
sys.argv = oldargv

# load FWLite C++ libraries
ROOT.gSystem.Load("libFWCoreFWLite.so");
ROOT.gSystem.Load("libDataFormatsFWLite.so");
ROOT.FWLiteEnabler.enable();

# load FWlite python libraries
from DataFormats.FWLite import Handle, Events

triggerBits, triggerBitLabel = Handle("edm::TriggerResults"), ("TriggerResults","","HLT")
triggerPrescales, triggerPrescaleLabel  = Handle("pat::PackedTriggerPrescales"), "patTrigger"

# open file (you can use 'edmFileUtil -d /store/whatever.root' to get the physical file name)
#events = Events("root://xrootd-cms.infn.it//store/mc/RunIISpring15DR74/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/Asympt50nsRecodebug_MCRUN2_74_V9A-v1/10000/068D678E-2902-E511-8710-0026B95CE867.root")
#events = Events("root://xrootd-cms.infn.it//store/mc/RunIISpring15DR74/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v2/00000/0AD1B416-AC0C-E511-9405-002590574604.root")
#events = Events("root://xrootd-cms.infn.it//store/user/algomez/RPVSt100tojj_13TeV_pythia8/RunIISpring15DR74_MiniAOD_v2/150613_105255/0002/RPVSt100tojj_13TeV_pythia8_MiniAOD_Asympt25ns_2164.root")
events = Events("root://xrootd-cms.infn.it//store/user/algomez/RPVSt200tojj_13TeV_pythia8/RunIISpring15DR74_MiniAOD_Asympt25ns/150703_072913/0000/RPVSt200tojj_13TeV_pythia8_MiniAOD_Asympt25ns_409.root")

for iev,event in enumerate(events):
    event.getByLabel(triggerBitLabel, triggerBits)
    event.getByLabel(triggerPrescaleLabel, triggerPrescales)

    print "\nEvent %d: run %6d, lumi %4d, event %12d" % (iev,event.eventAuxiliary().run(), event.eventAuxiliary().luminosityBlock(),event.eventAuxiliary().event())
    print "\n === TRIGGER PATHS ==="
    names = event.object().triggerNames(triggerBits.product())
    for i in xrange(triggerBits.product().size()):
        print "Trigger ", names.triggerName(i), ", prescale ", triggerPrescales.product().getPrescaleForIndex(i), ": ", ("PASS" if triggerBits.product().accept(i) else "fail (or not run)") 

    if iev > 10: break
