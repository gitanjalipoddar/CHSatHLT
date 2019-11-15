// system include files
#include <memory>
#include <cmath>
#include <TH1.h>
#include <TH2.h>
#include <TLorentzVector.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "PUHLT/PUmitigationatHLT/interface/CommonVariablesStructure.h"

using namespace edm;
using namespace std;
using namespace reco;
using namespace pat;

class TriggerEfficienciesfromMenu : public EDAnalyzer {

	public:
		explicit TriggerEfficienciesfromMenu(const ParameterSet&);
      		static void fillDescriptions(ConfigurationDescriptions & descriptions);
		~TriggerEfficienciesfromMenu() {}

	private:
		virtual void analyze(const Event&, const EventSetup&) override;
      		virtual void beginJob() override;

	EDGetTokenT<TriggerResults> triggerBits_;
	//EDGetTokenT<trigger::TriggerFilterObjectWithRefs> triggerObjects_;
	EDGetTokenT<PFJetCollection> triggerObjects_;
	EDGetTokenT<PFJetCollection> recoJetToken_;
	EDGetTokenT<PFJetCollection> patJetToken_;
	string baseTrigger_;
    vector<string> triggerPass_;
    vector<int> triggerOverlap_;
    vector<int> triggerOverlapBase_;
	double recojetPt_;
	bool AK8jets_;
	bool DEBUG_;

	Service<TFileService> fs_;
	map< string, TH1D* > histos1D_;
	map< string, TH2D* > histos2D_;
};

TriggerEfficienciesfromMenu::TriggerEfficienciesfromMenu(const ParameterSet& iConfig):
	triggerBits_(consumes<TriggerResults>(iConfig.getParameter<InputTag>("bits"))),
	//triggerObjects_(consumes<trigger::TriggerFilterObjectWithRef>(iConfig.getParameter<InputTag>("objects"))),
	triggerObjects_(consumes<PFJetCollection>(iConfig.getParameter<InputTag>("objects"))),
	recoJetToken_(consumes<PFJetCollection>(iConfig.getParameter<InputTag>("recoJets"))),
	patJetToken_(consumes<PFJetCollection>(iConfig.getParameter<InputTag>("patJets")))
{
	baseTrigger_ = iConfig.getParameter<string>("baseTrigger");
	triggerPass_ = iConfig.getParameter<vector<string>>("triggerPass");
	triggerOverlap_ = iConfig.getParameter<vector<int>>("triggerOverlap");
	triggerOverlapBase_ = iConfig.getParameter<vector<int>>("triggerOverlapBase");
	recojetPt_ = iConfig.getParameter<double>("recojetPt");
	AK8jets_ = iConfig.getParameter<bool>("AK8jets");
	DEBUG_ = iConfig.getParameter<bool>("DEBUG");
}

void TriggerEfficienciesfromMenu::analyze(const Event& iEvent, const EventSetup& iSetup) {

	Handle<TriggerResults> triggerBits;
	//Handle<trigger::TriggerFilterObjectWithRefs> triggerObjects;
	Handle<PFJetCollection> triggerObjects;
	Handle<PFJetCollection> recojets;
	Handle<PFJetCollection> patjets;

	iEvent.getByToken(triggerBits_, triggerBits);
	iEvent.getByToken(triggerObjects_, triggerObjects);
	iEvent.getByToken(recoJetToken_, recojets);
	iEvent.getByToken(patJetToken_, patjets);

	const TriggerNames &names = iEvent.triggerNames(*triggerBits);

    // Checking if the triggers were fired
	bool baseTrigger = 0;
	bool ORTriggers = 0;
	vector<bool> triggersFired;
	for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
		if (DEBUG_) LogWarning("all triggers") << names.triggerName(i) << " " <<  triggerBits->accept(i);
		if (TString(names.triggerName(i)).Contains(baseTrigger_) && (triggerBits->accept(i)))  baseTrigger = true;
		for (size_t t = 0; t < triggerPass_.size(); t++) {
			if (TString(names.triggerName(i)).Contains(triggerPass_[t]) && (triggerBits->accept(i))) triggersFired.push_back( true );
			else triggersFired.push_back( false );
		}
	}
	ORTriggers = any_of(triggersFired.begin(), triggersFired.end(), [](bool v) { return v; }); 
	triggersFired.clear();
	if (DEBUG_) LogWarning("trigger fired") << "Based " << baseTrigger << " OR " << ORTriggers;
	if ( TString(baseTrigger_).Contains("empty") ) baseTrigger = true;


	///// HLT OBJECTS
	if ( ORTriggers ) { // because not every event has triggerObjects

		double hltHT = 0;
        int numHLTJets = 0;
		double hltHTpt10 = 0;
        int numHLTJetspt10 = 0;
        
        for ( auto const& triggerJet : *triggerObjects ) {

            // first cleaning since we can not go lower in pt anyway
			if( triggerJet.pt() < 5 ) continue;
			if( TMath::Abs(triggerJet.eta()) > 2.5 ) continue;
            if (DEBUG_) LogWarning("trigger jet") << "\tTrigger object Pt:  pt " << triggerJet.pt()  << ", eta " << triggerJet.eta() << ", phi " << triggerJet.phi() << ", mass " << triggerJet.mass();

            // plotting for the basic case
            hltHT += triggerJet.pt();
            numHLTJets+=1;
            histos1D_[ "hltJetPt" ]->Fill( triggerJet.pt() );
            histos1D_[ "hltJetEta" ]->Fill( triggerJet.eta() );

            /* dont remove it since it can help for later	
                if (DEBUG_) LogWarning("trigger mass") << "\tTrigger object Mass:  pt " << obj.pt() << ", eta " << obj.eta() << ", phi " << obj.phi() << ", mass " << obj.mass(); 
                hltMass = obj.mass();
                histos1D_[ "hltJetMass" ]->Fill( hltMass );
            }*/
			if( triggerJet.pt() < 10 ) continue;
            hltHTpt10 += triggerJet.pt();
            numHLTJetspt10+=1;
            histos1D_[ "hltJetPt_pt10" ]->Fill( triggerJet.pt() );
            histos1D_[ "hltJetEta_pt10" ]->Fill( triggerJet.eta() );

        }

        //histos2D_[ "hltJetPtvsMass" ]->Fill( hltPt, hltMass );
        histos1D_[ "hltJetHT" ]->Fill( hltHT );
        histos1D_[ "hltnumJets" ]->Fill( numHLTJets );
        histos1D_[ "hltJetHT_pt10" ]->Fill( hltHTpt10 );
        histos1D_[ "hltnumJets_pt10" ]->Fill( numHLTJetspt10 );
	    
        if ( baseTrigger || ORTriggers ) { // checking if both triggers passed

            /////////////////////////////////////////////////////////////////////////////////
            /// This is for recoJets
            double HT = 0;
            int k = 0;
            for (const reco::Jet &recojet : *recojets) {

                if( recojet.pt() < recojetPt_ ) continue;
                if( TMath::Abs(recojet.eta()) > 2.5 ) continue;
                if (DEBUG_) LogWarning("recojets") << recojet.pt();
                HT += recojet.pt();

                if (++k==1){
                    histos1D_[ "jet1Pt" ]->Fill( recojet.pt() );

                    // this will help later, but good check for now.
                    if ( baseTrigger ) {
                        histos1D_[ "jet1PtDenom" ]->Fill( recojet.pt() );
                        if ( ORTriggers ) histos1D_[ "jet1PtPassing" ]->Fill( recojet.pt() );
                    }
                }

            }
            histos1D_[ "HT" ]->Fill( HT );
            
            if ( baseTrigger ) {
                histos1D_[ "HTDenom" ]->Fill( HT );
                if ( ORTriggers ) histos1D_[ "HTPassing" ]->Fill( HT );
                if ( hltHT > 700 ) histos1D_[ "HTPassingHT700" ]->Fill( HT );
                if ( hltHT > 800 ) histos1D_[ "HTPassingHT800" ]->Fill( HT );
                if ( hltHT > 850 ) histos1D_[ "HTPassingHT850" ]->Fill( HT );
                if ( hltHT > 900 ) histos1D_[ "HTPassingHT900" ]->Fill( HT );
                if ( hltHT > 950 ) histos1D_[ "HTPassingHT950" ]->Fill( HT );
                if ( hltHT > 1000 ) histos1D_[ "HTPassingHT1000" ]->Fill( HT );
                if ( hltHT > 1050 ) histos1D_[ "HTPassingHT1050" ]->Fill( HT );

                /// for HT with pt >10
                if ( hltHTpt10 > 700 ) histos1D_[ "HTPassingHT700pt10" ]->Fill( HT );
                if ( hltHTpt10 > 800 ) histos1D_[ "HTPassingHT800pt10" ]->Fill( HT );
                if ( hltHTpt10 > 850 ) histos1D_[ "HTPassingHT850pt10" ]->Fill( HT );
                if ( hltHTpt10 > 900 ) histos1D_[ "HTPassingHT900pt10" ]->Fill( HT );
                if ( hltHTpt10 > 950 ) histos1D_[ "HTPassingHT950pt10" ]->Fill( HT );
                if ( hltHTpt10 > 1000 ) histos1D_[ "HTPassingHT1000pt10" ]->Fill( HT );
                if ( hltHTpt10 > 1050 ) histos1D_[ "HTPassingHT1050pt10" ]->Fill( HT );
            }
            /////////////////////////////////////////////////////////////////////////////////

            /////////////////////////////////////////////////////////////////////////////////
            /// This is for patJets
            double puppiHT = 0;
            int kk = 0;
            for (const reco::Jet &patjet : *patjets) {

                if( patjet.pt() < recojetPt_ ) continue;
                if( TMath::Abs(patjet.eta()) > 2.5 ) continue;
                if (DEBUG_) LogWarning("patjets") << patjet.pt();
                puppiHT += patjet.pt();

                if (++kk==1){
                    histos1D_[ "puppijet1Pt" ]->Fill( patjet.pt() );

                    // this will help later, but good check for now.
                    if ( baseTrigger ) {
                        histos1D_[ "puppijet1PtDenom" ]->Fill( patjet.pt() );
                        if ( ORTriggers ) histos1D_[ "puppijet1PtPassing" ]->Fill( patjet.pt() );
                    }
                }

            }
            histos1D_[ "puppiHT" ]->Fill( puppiHT );
            
            if ( baseTrigger ) {
                histos1D_[ "puppiHTDenom" ]->Fill( puppiHT );
                if ( ORTriggers ) histos1D_[ "puppiHTPassing" ]->Fill( puppiHT );
                if ( hltHT > 700 ) histos1D_[ "puppiHTPassingHT700" ]->Fill( puppiHT );
                if ( hltHT > 800 ) histos1D_[ "puppiHTPassingHT800" ]->Fill( puppiHT );
                if ( hltHT > 850 ) histos1D_[ "puppiHTPassingHT850" ]->Fill( puppiHT );
                if ( hltHT > 900 ) histos1D_[ "puppiHTPassingHT900" ]->Fill( puppiHT );
                if ( hltHT > 950 ) histos1D_[ "puppiHTPassingHT950" ]->Fill( puppiHT );
                if ( hltHT > 1000 ) histos1D_[ "puppiHTPassingHT1000" ]->Fill( puppiHT );
                if ( hltHT > 1050 ) histos1D_[ "puppiHTPassingHT1050" ]->Fill( puppiHT );

                /// for puppiHT with pt >10
                if ( hltHTpt10 > 700 ) histos1D_[ "puppiHTPassingHT700pt10" ]->Fill( puppiHT );
                if ( hltHTpt10 > 800 ) histos1D_[ "puppiHTPassingHT800pt10" ]->Fill( puppiHT );
                if ( hltHTpt10 > 850 ) histos1D_[ "puppiHTPassingHT850pt10" ]->Fill( puppiHT );
                if ( hltHTpt10 > 900 ) histos1D_[ "puppiHTPassingHT900pt10" ]->Fill( puppiHT );
                if ( hltHTpt10 > 950 ) histos1D_[ "puppiHTPassingHT950pt10" ]->Fill( puppiHT );
                if ( hltHTpt10 > 1000 ) histos1D_[ "puppiHTPassingHT1000pt10" ]->Fill( puppiHT );
                if ( hltHTpt10 > 1050 ) histos1D_[ "puppiHTPassingHT1050pt10" ]->Fill( puppiHT );
            }
        }
    }
	////////////////////////////////////////////////////////////////////////////////////////////////////

    /*///// checking overlap, dont remove it, it will help later
    if ( triggerOverlapBase_[0] != triggerOverlap_[0] ) {

        bool overlapBase = 0;
        vector<bool> vectorOverlapBase;
        for (size_t b = 0; b < triggerOverlapBase_.size(); b++) {
            vectorOverlapBase.push_back( triggersFired[ triggerOverlapBase_[b] ] );	
            //LogWarning("base") <<  triggerOverlapBase_[b] << " " << triggersFired[triggerOverlapBase_[b]];
        }
        overlapBase = any_of(vectorOverlapBase.begin(), vectorOverlapBase.end(), [](bool v) { return v; }); 
        //LogWarning("baseAll") <<  overlapBase;

        bool overlap = 0;
        vector<bool> vectorOverlap;
        for (size_t b = 0; b < triggerOverlap_.size(); b++) {
            vectorOverlap.push_back( triggersFired[ triggerOverlap_[b] ] );	
            //LogWarning("no base") <<  triggerOverlap_[b] << " " << triggersFired[triggerOverlap_[b]];
        }
        overlap = any_of(vectorOverlap.begin(), vectorOverlap.end(), [](bool v) { return v; }); 
        //LogWarning("no baseAll") <<  overlap;
        
        if ( overlapBase && overlap ) {
            histos2D_[ "jet1SDMassvsHT11" ]->Fill( jet1SoftdropMass, HT );
            histos2D_[ "jet1SDMassvsPt11" ]->Fill( jet1SoftdropMass, jet1Pt );
        } else if (!overlapBase && overlap ) {
            histos2D_[ "jet1SDMassvsHT01" ]->Fill( jet1SoftdropMass, HT );
            histos2D_[ "jet1SDMassvsPt01" ]->Fill( jet1SoftdropMass, jet1Pt );
        } else if (overlapBase && !overlap ) {
            histos2D_[ "jet1SDMassvsHT10" ]->Fill( jet1SoftdropMass, HT );
            histos2D_[ "jet1SDMassvsPt10" ]->Fill( jet1SoftdropMass, jet1Pt );
        } else { 
            histos2D_[ "jet1SDMassvsHT00" ]->Fill( jet1SoftdropMass, HT );
            histos2D_[ "jet1SDMassvsPt00" ]->Fill( jet1SoftdropMass, jet1Pt );
        }
    }*/
}

void TriggerEfficienciesfromMenu::beginJob() {

	histos1D_[ "hltJetPt" ] = fs_->make< TH1D >( "hltJetPt", "hltJetPt", 2000, 0., 2000. );
	//histos1D_[ "hltJetMass" ] = fs_->make< TH1D >( "hltJetMass", "hltJetMass", 2000, 0., 2000. );
	histos1D_[ "hltJetEta" ] = fs_->make< TH1D >( "hltJetEta", "hltJetEta", 100, -5, 5 );
	histos1D_[ "hltJetHT" ] = fs_->make< TH1D >( "hltJetHT", "hltJetHT", 5000, 0., 5000. );
	histos1D_[ "hltnumJets" ] = fs_->make< TH1D >( "hltnumJets", "hltnumJets", 20, 0., 20. );

	histos1D_[ "hltJetPt_pt10" ] = fs_->make< TH1D >( "hltJetPt_pt10", "hltJetPt_pt10", 2000, 0., 2000. );
	histos1D_[ "hltJetEta_pt10" ] = fs_->make< TH1D >( "hltJetEta_pt10", "hltJetEta_pt10", 100, -5, 5 );
	histos1D_[ "hltJetHT_pt10" ] = fs_->make< TH1D >( "hltJetHT_pt10", "hltJetHT_pt10", 5000, 0., 5000. );
	histos1D_[ "hltnumJets_pt10" ] = fs_->make< TH1D >( "hltnumJets_pt10", "hltnumJets_pt10", 20, 0., 20. );

	//histos2D_[ "hltJetPtvsMass" ] = fs_->make< TH2D >( "hltJetPtvsMass", "hltJetPtvsMass", 2000, 0., 2000., 2000, 0., 2000. );

	histos1D_[ "jet1Pt" ] = fs_->make< TH1D >( "jet1Pt", "jet1Pt", 1000, 0., 1000. );
	histos1D_[ "HT" ] = fs_->make< TH1D >( "HT", "HT", 100, 0., 2000. );


	histos1D_[ "jet1PtDenom" ] = fs_->make< TH1D >( "jet1PtDenom", "jet1PtDenom", 1000, 0., 1000. );
	histos1D_[ "jet1PtPassing" ] = fs_->make< TH1D >( "jet1PtPassing", "jet1PtPassing", 1000, 0., 1000. );
	histos1D_[ "HTDenom" ] = fs_->make< TH1D >( "HTDenom", "HTDenom", 2000, 0., 2000. );
	histos1D_[ "HTPassing" ] = fs_->make< TH1D >( "HTPassing", "HTPassing", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT700" ] = fs_->make< TH1D >( "HTPassingHT700", "HTPassingHT700", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT800" ] = fs_->make< TH1D >( "HTPassingHT800", "HTPassingHT800", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT850" ] = fs_->make< TH1D >( "HTPassingHT850", "HTPassingHT850", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT900" ] = fs_->make< TH1D >( "HTPassingHT900", "HTPassingHT900", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT950" ] = fs_->make< TH1D >( "HTPassingHT950", "HTPassingHT950", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1000" ] = fs_->make< TH1D >( "HTPassingHT1000", "HTPassingHT1000", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1050" ] = fs_->make< TH1D >( "HTPassingHT1050", "HTPassingHT1050", 2000, 0., 2000. );

	histos1D_[ "HTPassingHT700pt10" ] = fs_->make< TH1D >( "HTPassingHT700pt10", "HTPassingHT700pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT800pt10" ] = fs_->make< TH1D >( "HTPassingHT800pt10", "HTPassingHT800pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT850pt10" ] = fs_->make< TH1D >( "HTPassingHT850pt10", "HTPassingHT850pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT900pt10" ] = fs_->make< TH1D >( "HTPassingHT900pt10", "HTPassingHT900pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT950pt10" ] = fs_->make< TH1D >( "HTPassingHT950pt10", "HTPassingHT950pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1000pt10" ] = fs_->make< TH1D >( "HTPassingHT1000pt10", "HTPassingHT1000pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1050pt10" ] = fs_->make< TH1D >( "HTPassingHT1050pt10", "HTPassingHT1050pt10", 2000, 0., 2000. );

    /////////////////////////////////////////////////////////////
	histos1D_[ "puppijet1Pt" ] = fs_->make< TH1D >( "puppijet1Pt", "puppijet1Pt", 1000, 0., 1000. );
	histos1D_[ "puppiHT" ] = fs_->make< TH1D >( "puppiHT", "puppiHT", 100, 0., 2000. );


	histos1D_[ "puppijet1PtDenom" ] = fs_->make< TH1D >( "puppijet1PtDenom", "puppijet1PtDenom", 1000, 0., 1000. );
	histos1D_[ "puppijet1PtPassing" ] = fs_->make< TH1D >( "puppijet1PtPassing", "puppijet1PtPassing", 1000, 0., 1000. );
	histos1D_[ "puppiHTDenom" ] = fs_->make< TH1D >( "puppiHTDenom", "puppiHTDenom", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassing" ] = fs_->make< TH1D >( "puppiHTPassing", "puppiHTPassing", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT700" ] = fs_->make< TH1D >( "puppiHTPassingHT700", "puppiHTPassingHT700", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT800" ] = fs_->make< TH1D >( "puppiHTPassingHT800", "puppiHTPassingHT800", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT850" ] = fs_->make< TH1D >( "puppiHTPassingHT850", "puppiHTPassingHT850", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT900" ] = fs_->make< TH1D >( "puppiHTPassingHT900", "puppiHTPassingHT900", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT950" ] = fs_->make< TH1D >( "puppiHTPassingHT950", "puppiHTPassingHT950", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1000" ] = fs_->make< TH1D >( "puppiHTPassingHT1000", "puppiHTPassingHT1000", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1050" ] = fs_->make< TH1D >( "puppiHTPassingHT1050", "puppiHTPassingHT1050", 2000, 0., 2000. );

	histos1D_[ "puppiHTPassingHT700pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT700pt10", "puppiHTPassingHT700pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT800pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT800pt10", "puppiHTPassingHT800pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT850pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT850pt10", "puppiHTPassingHT850pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT900pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT900pt10", "puppiHTPassingHT900pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT950pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT950pt10", "puppiHTPassingHT950pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1000pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT1000pt10", "puppiHTPassingHT1000pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1050pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT1050pt10", "puppiHTPassingHT1050pt10", 2000, 0., 2000. );



	///// Sumw2 all the histos
	for( auto const& histo : histos1D_ ) histos1D_[ histo.first ]->Sumw2();
	for( auto const& histo : histos2D_ ) histos2D_[ histo.first ]->Sumw2();
}

void TriggerEfficienciesfromMenu::fillDescriptions(ConfigurationDescriptions & descriptions) {

	ParameterSetDescription desc;
	desc.add<InputTag>("bits", 	InputTag("TriggerResults", "", "HLT2"));
	desc.add<InputTag>("objects", 	InputTag("slimmedPatTrigger"));
	desc.add<string>("baseTrigger", 	"HLT_PFHT800");
	desc.add<double>("recojetPt", 	50);
	desc.add<bool>("AK8jets", 	true);
	desc.add<bool>("DEBUG", 	false);
	desc.add<InputTag>("recoJets", 	InputTag("slimmedJetsAK8"));
	desc.add<InputTag>("patJets", 	InputTag("slimmedJetsAK8"));
	vector<string> HLTPass;
	HLTPass.push_back("HLT_AK8PFHT650_TrimR0p1PT0p03Mass50");
	desc.add<vector<string>>("triggerPass",	HLTPass);
	vector<int> HLTOverlapBase;
	HLTOverlapBase.push_back(0);
	desc.add<vector<int>>("triggerOverlapBase",	HLTOverlapBase);
	vector<int> HLTOverlap;
	HLTOverlap.push_back(0);
	desc.add<vector<int>>("triggerOverlap",	HLTOverlap);
	descriptions.addDefault(desc);
}
      
//define this as a plug-in
DEFINE_FWK_MODULE(TriggerEfficienciesfromMenu);
