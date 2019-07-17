import ROOT
import sys
from DataFormats.FWLite import Events, Handle
import argparse
ROOT.gSystem.Load("libFWCoreFWLite.so");
ROOT.FWLiteEnabler.enable();
ROOT.gSystem.Load("libDataFormatsFWLite.so");
ROOT.gSystem.Load("libDataFormatsPatCandidates.so");



def simpleAnalyzer():
    """docstring for simpleAnalyzer"""

    outputFile = ROOT.TFile( args.outputFileName, 'RECREATE' )

    histos['numJets'] = ROOT.TH1F('numJets', ';Number of jets', 20, 0, 20)
    histos['numJetsCHS'] = ROOT.TH1F('numJetsCHS', ';Number of jets', 20, 0, 20)
    histos['numJetsSK'] = ROOT.TH1F('numJetsSK', ';Number of jets', 20, 0, 20)
    histos['jetPt'] = ROOT.TH1F('jetPt', ';jet pt (GeV)', 100, 0, 1000)
    histos['jetPtSK'] = ROOT.TH1F('jetPtSK', ';jet pt (GeV)', 100, 0, 1000)
    histos['jetPtCHS'] = ROOT.TH1F('jetPtCHS', ';jet pt (GeV)', 100, 0, 1000)
    histos['jetEta'] = ROOT.TH1F('jetEta', ';jet eta', 120, -3, 3)
    histos['jetEtaSK'] = ROOT.TH1F('jetEtaSK', ';jet eta', 120, -3, 3)
    histos['jetEtaCHS'] = ROOT.TH1F('jetEtaCHS', ';jet eta', 120, -3, 3)
    histos['jetHT'] = ROOT.TH1F('jetHT', ';HT (GeV)', 500, 0, 5000)
    histos['jetHTSK'] = ROOT.TH1F('jetHTSK', ';HT (GeV)', 500, 0, 5000)
    histos['jetHTCHS'] = ROOT.TH1F('jetHTCHS', ';HT (GeV)', 500, 0, 5000)

    for h in histos: histos[h].Sumw2()

    ##triggerLabel, triggerSummary = ("hltTriggerSummaryAOD::TEST"), Handle("trigger::TriggerEvent")
    #ak4PFJetLabel, akPFJet = "hltAK4PFJets::TEST", Handle("std::vector<reco::PFJet>")
    ak4PFJetLabel, akPFJet = "hltAK4PFJetsCorrected::TEST", Handle("std::vector<reco::PFJet>")
    #ak4PFCHSJetLabel, akPFCHSJet = "hltAK4PFJetsForCHS::TEST", Handle("std::vector<reco::PFJet>")
    ak4PFCHSJetLabel, akPFCHSJet = "hltAK4PFCHSJetsCorrected::TEST", Handle("std::vector<reco::PFJet>")
    #ak4PFSKJetLabel, akPFSKJet = "hltAK4PFJetsForSK::TEST", Handle("std::vector<reco::PFJet>")
    ak4PFSKJetLabel, akPFSKJet = "hltAK4PFSKJetsCorrected::TEST", Handle("std::vector<reco::PFJet>")

    events = Events(args.inputFile)

    for iev,event in enumerate(events):

        if args.debug: print "\nEvent %d: run %6d, lumi %4d, event %12d" % (iev,event.eventAuxiliary().run(), event.eventAuxiliary().luminosityBlock(),event.eventAuxiliary().event())

        ##### Plain jets
        event.getByLabel( ak4PFJetLabel, akPFJet )
        if ( akPFJet.isValid() ): # not all the events have akPFJet collection
            numJets = 0
            ak4HT = 0
            for i,ijet in enumerate(akPFJet.product()):
                if ijet.pt() < minJetPt or abs(ijet.eta())>minJetEta: continue
                if args.debug: print "jet %3d: pt %5.1f, eta %+4.2f" % (i, ijet.pt(), ijet.eta() )
                histos['jetPt'].Fill( ijet.pt() )
                histos['jetEta'].Fill( ijet.eta() )
                numJets = numJets+1
                ak4HT+= ijet.pt()
            histos['numJets'].Fill( numJets )
            histos['jetHT'].Fill( ak4HT )

        ##### SK jets
        event.getByLabel( ak4PFSKJetLabel, akPFSKJet )
        if ( akPFSKJet.isValid() ): # not all the events have akPFSKJet collection
            numSKJets = 0
            ak4SKHT = 0
            for i,ijet in enumerate(akPFSKJet.product()):
                if ijet.pt() < minJetPt or abs(ijet.eta())>minJetEta: continue
                if args.debug: print "jet %3d: pt %5.1f, eta %+4.2f" % (i, ijet.pt(), ijet.eta() )
                histos['jetPtSK'].Fill( ijet.pt() )
                histos['jetEtaSK'].Fill( ijet.eta() )
                numSKJets = numSKJets+1
                ak4SKHT+= ijet.pt()
            histos['numJetsSK'].Fill( numSKJets )
            histos['jetHTSK'].Fill( ak4SKHT )

        ##### CHS jets
        event.getByLabel( ak4PFCHSJetLabel, akPFCHSJet )
        if ( akPFCHSJet.isValid() ): # not all the events have akPFCHSJet collection
            numCHSJets = 0
            ak4CHSHT = 0
            for i,ijet in enumerate(akPFCHSJet.product()):
                if ijet.pt() < minJetPt or abs(ijet.eta())>minJetEta: continue
                if args.debug: print "jet %3d: pt %5.1f, eta %+4.2f" % (i, ijet.pt(), ijet.eta() )
                histos['jetPtCHS'].Fill( ijet.pt() )
                histos['jetEtaCHS'].Fill( ijet.eta() )
                numCHSJets = numCHSJets+1
                ak4CHSHT+= ijet.pt()
            histos['numJetsCHS'].Fill( numCHSJets )
            histos['jetHTCHS'].Fill( ak4CHSHT )


    outputFile.Write()
    ##### Closing
    print 'Writing output file: '+ args.outputFileName
    outputFile.Close()


################################################
if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--inputFile', action='store', default='/afs/cern.ch/user/a/algomez/cernbox/tmp/outputCHSSK.root', dest='inputFile', help='Name of the input file.' )
    parser.add_argument('-o', '--outputFile', action='store', default='outputSimpleAnalyzer.root', dest='outputFileName', help='Name of the output file.' )
    parser.add_argument('-b', '--debug', action='store_false', default=False, dest='debug', help='Debug mode' )

    try: args = parser.parse_args()
    except:
        parser.print_help()
        sys.exit(0)

    minJetPt = 30
    minJetEta = 2.5
    histos = {}
    simpleAnalyzer()
