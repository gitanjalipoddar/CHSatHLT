// -*- C++ -*-
//
// Package:    MyTrigger/myTriggerStudies
// Class:      TriggerEfficiencyPlots.cc
// 
/**\class TriggerEfficiencyPlots.cc TriggerEfficiencyPlots.cc.cc MyTrigger/myTriggerStudies/plugins/TriggerEfficiencyPlots.cc.cc

 Description: Trigger efficiency plots

*/
//
// Original Author:  Alejandro Gomez Espinosa
//         Created:  Wed, 27 Aug 2014 19:40:39 GMT
//
//

// system include files
#include <memory>

// user include files
#include "PUHLT/CHSatHLT/interface/CommonVariablesStructure.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/METReco/interface/MET.h"

#include "TMath.h"
#include "TH2D.h"
#include "TH1D.h"
#include <TLorentzVector.h>

using namespace edm;
using namespace std;
using namespace reco;

//
// class declaration
//

class TriggerEfficiencyPlots : public edm::EDAnalyzer {
	public:
		explicit TriggerEfficiencyPlots(const edm::ParameterSet&);
		~TriggerEfficiencyPlots();

		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
		static bool compare_JetMass(const TLorentzVector jet1, const TLorentzVector jet2){
			return ( jet1.M() > jet2.M() );
		}


	private:
		virtual void beginJob() override;
		virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
		virtual void endJob() override;

  		bool RecoHLTMatching(const edm::Event&,double recoele_eta, double recoele_phi, edm::InputTag IT_filter, double dRmatching = 0.3);


		// ----------member data ---------------------------
		std::map< std::string, TH2D* > histos2D_;
		std::map< std::string, TH1D* > histos1D_;

		edm::EDGetTokenT<edm::TriggerResults> triggerResultsToken; 
  		edm::EDGetTokenT<trigger::TriggerEvent> triggerObjectsToken;
		edm::EDGetTokenT<reco::PFJetCollection> recoJetsToken;
		edm::EDGetTokenT<pat::JetCollection> patJetsToken;
		edm::EDGetTokenT<reco::VertexCollection> offlinePV;
		edm::EDGetTokenT<reco::PFJetCollection> htHLTToken;
		edm::EDGetTokenT<trigger::TriggerFilterObjectWithRefs> m_theTriggerMETToken;
		std::string triggerPath;
		double minPt;
		double minHT;
		double minMass;

		HLTConfigProvider hltConfig;
		int triggerBit;
		std::vector<reco::METRef> jetRefVec;
		int TriggerType_;

};



TriggerEfficiencyPlots::TriggerEfficiencyPlots(const edm::ParameterSet& iConfig):
	triggerResultsToken(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag> ( "triggerResults"))),
  	triggerObjectsToken(consumes<trigger::TriggerEvent>(iConfig.getParameter<edm::InputTag> ("triggerObjects"))),
	recoJetsToken(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag> ( "recoJets" ))),   			// Obtain inputs
	//patJetsToken(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag> ( "patJets" ))),   			// Obtain inputs
	offlinePV(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag> ( "primaryVertex" )))
	//htHLTToken(consumes<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag> ( "htHLT" )))
	//m_theTriggerMETToken(consumes<trigger::TriggerFilterObjectWithRefs>(iConfig.getParameter<edm::InputTag> ( "METFilter" )))
{
	triggerPath		= iConfig.getParameter<std::string> ( "triggerPath" );   			// Obtain inputs
	minHT			= iConfig.getParameter<double> ( "minHT" );   			// Obtain inputs
	minPt			= iConfig.getParameter<double> ( "minPt" );   			// Obtain inputs
	minMass			= iConfig.getParameter<double> ( "minMass" );   			// Obtain inputs
}


TriggerEfficiencyPlots::~TriggerEfficiencyPlots()
{
}


//
// member functions
//

//In the next few lines one loops over all the trigger objects (corresponding to a given filter) and check whether one of them matches the reco object under study
/*bool TriggerEfficiencyPlots::RecoHLTMatching(const edm::Event& iEvent, double recoeta, double recophi, edm::InputTag filterNameTEST, double dRmatching){

	edm::Handle<trigger::TriggerEvent> trigEvent;
	iEvent.getByToken(triggerObjectsToken ,trigEvent);
	edm::InputTag trigEventTag("hltTriggerSummaryAOD","","HLT");
	std::string filterName("hltPFHT900Jet30"); 

	trigger::size_type filterIndex = trigEvent->filterIndex( edm::InputTag(filterName,"",trigEventTag.process()));
	//trigger::size_type filterIndex = trigEvent->filterIndex( filterName ); // edm::InputTag(filterName,"",trigEventTag.process()));
	LogWarning("test") << "trigger filterIndex " << filterIndex << " " <<  trigEvent->sizeFilters() ;

	if( filterIndex < trigEvent->sizeFilters() ){ 
		
		const trigger::Keys& trigKeys = trigEvent->filterKeys(filterIndex); 
		const trigger::TriggerObjectCollection & trigObjColl(trigEvent->getObjects());
		//now loop of the trigger objects passing filter
		for( trigger::Keys::const_iterator keyIt=trigKeys.begin(); keyIt!=trigKeys.end(); ++keyIt ){ 

			const trigger::TriggerObject& obj = trigObjColl[*keyIt];
			LogWarning("trigger obj") << obj.pt() << " " << obj.eta();
		}
	}


	//trigger::TriggerObjectCollection selectedObjects;

	//LogWarning("test") << "triggerObjectsSummary";
	if (triggerObjectsSummary.isValid()) {

		size_t filterIndex = (*triggerObjectsSummary).filterIndex(IT_filter);
		trigger::TriggerObjectCollection allTriggerObjects = triggerObjectsSummary->getObjects();
		LogWarning("test") << "trigger filterIndex " << filterIndex << " " <<  (*triggerObjectsSummary).sizeFilters() ;
		if (filterIndex <= (*triggerObjectsSummary).sizeFilters()) { //check if the trigger object is present 
			const trigger::Keys &keys = (*triggerObjectsSummary).filterKeys(filterIndex);
			LogWarning("test") << "check if trigger object present";
			for (size_t j = 0; j < keys.size(); j++) {
				trigger::TriggerObject foundObject = (allTriggerObjects)[keys[j]];
				LogWarning("test") << recoeta << " " << recophi << " " << foundObject.eta() << " " << foundObject.phi();
				if( deltaR( recoeta, recophi, foundObject.eta(), foundObject.phi() ) < dRmatching )return true;
			}
		}
	}
	return false;
}
*/

// ------------ method called for each event  ------------
void TriggerEfficiencyPlots::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

	using namespace edm;
	using namespace std;

	bool changedConfig = false;
	if (!hltConfig.init(iEvent.getRun(), iSetup, "HLT2", changedConfig)) {
		cout << "Initialization of HLTConfigProvider failed!!" << endl;
		return;
	}
	if (changedConfig){
		std::cout << "the curent menu is " << hltConfig.tableName()  << std::endl;
		triggerBit = -1;
		for (size_t j = 0; j < hltConfig.triggerNames().size(); j++) {
			std::cout << TString(hltConfig.triggerNames()[j]) << std::endl;
			if (TString(hltConfig.triggerNames()[j]).Contains(triggerPath)) triggerBit = j;
		}
		std::cout << triggerBit << std::endl;
		if (triggerBit == -1) cout << "HLT path not found" << endl;

	}

	//bool triggerPass =  checkORListOfTriggerBitsMiniAOD( TriggerNames triggerNames, Handle<TriggerResults> triggerBits, vector<string>  triggerPass  );

	//open the trigger summary
	edm::Handle<edm::TriggerResults> triggerResults;
	iEvent.getByToken(triggerResultsToken, triggerResults);

	edm::Handle<reco::VertexCollection> vertices;
	iEvent.getByToken(offlinePV, vertices);
	//edm::LogWarning("test")<< vertices->size();
	//int NPV = vertices->size();
	histos1D_[ "numPV" ]->Fill( vertices->size() );

	
	/*edm::Handle<trigger::TriggerFilterObjectWithRefs> TriggeredMET;
	iEvent.getByToken(m_theTriggerMETToken,TriggeredMET);	
	jetRefVec.clear();
	TriggeredMET->getObjects( trigger::TriggerMET, jetRefVec);

	for(auto & iCalo : jetRefVec) {  
		LogWarning("HT") << iCalo->pt();
	}*/
		

	/*edm::Handle<reco::PFJetCollection> htHLT;
	iEvent.getByToken(htHLTToken, htHLT);
	for(const reco::PFJet &iht : *htHLT){
		LogWarning("HT") << iht.pt();
	}*/

	/*edm::InputTag triggerSummaryLabel_ = edm::InputTag("hltTriggerSummaryAOD", "", "HLT");
	edm::Handle<trigger::TriggerEvent> triggerSummary;
	iEvent.getByToken(triggerSummaryLabel_, triggerSummary);*/

	//// CHS from AOD
	edm::Handle<reco::PFJetCollection> recoJets;
	iEvent.getByToken(recoJetsToken,recoJets);
	TLorentzVector reco1Jet;
	double recoHT = 0;
	int nrecoJets =0;
	for(const reco::PFJet &ijet : *recoJets ){
		if ( ijet.pt() < minPt || abs( ijet.eta() ) > 2.4 ) continue;
		recoHT += ijet.pt();
		if ( (nrecoJets++) == 1 ) {
			reco1Jet.SetPtEtaPhiE( ijet.pt(), ijet.eta(), ijet.phi(), ijet.energy() );
			histos1D_[ "recojet1PtDenom" ]->Fill( ijet.pt() );
			if (triggerResults->accept(triggerBit)){
				histos1D_[ "recojet1PtPassing" ]->Fill( ijet.pt() );
			}
		}
		
		//bool matchedWithHLT = RecoHLTMatching(iEvent, ijet.eta(), ijet.phi(), edm::InputTag("hltPFHT1000Jet30")); //hlt1AK8PFJetsTrimR0p1PT0p03Mass50","","HLT"));
		//if(matchedWithHLT) histos1D_[ "patJetMass" ]->Fill( ijet.mass() );
	}
	//edm::LogWarning("test")<< hlt1Jet.M() << " " << hlt1Jet.Pt() << " " << hltHT ;

	/*/// Puppi from toolbox
	edm::Handle<pat::JetCollection> patJets;
	iEvent.getByToken(patJetsToken,patJets);
	TLorentzVector pat1Jet;
	double patHT = 0;
	int npatJets =0;
	for(const pat::Jet &ijet : *patJets ){
		if ( ijet.pt() < minPt || abs( ijet.eta() ) > 2.4 ) continue;
		patHT += ijet.pt();
		if ( (npatJets++) == 1 ) {
			pat1Jet.SetPtEtaPhiE( ijet.pt(), ijet.eta(), ijet.phi(), ijet.energy() );
			histos1D_[ "patjet1PtDenom" ]->Fill( ijet.pt() );
			if (triggerResults->accept(triggerBit)){
				histos1D_[ "patjet1PtPassing" ]->Fill( ijet.pt() );
			}
		}
		
	}*/

}


// ------------ method called once each job just before starting event loop  ------------
void TriggerEfficiencyPlots::beginJob() {

	edm::Service< TFileService > fileService;


	histos1D_[ "recojet1PtDenom" ] = fileService->make< TH1D >( "recojet1PtDenom", "recojet1PtDenom", 2000, 0., 2000);
	histos1D_[ "recojet1PtPassing" ] = fileService->make< TH1D >( "recojet1PtPassing", "recojet1PtPassing", 2000, 0., 2000);

	histos1D_[ "patjet1PtDenom" ] = fileService->make< TH1D >( "patjet1PtDenom", "patjet1PtDenom", 2000, 0., 2000);
	histos1D_[ "patjet1PtPassing" ] = fileService->make< TH1D >( "patjet1PtPassing", "patjet1PtPassing", 2000, 0., 2000);



	histos1D_[ "HTDenom" ] = fileService->make< TH1D >( "HTDenom", "HT", 100, 0., 2000);
	histos1D_[ "HTDenom" ]->SetXTitle( "HT [GeV]" );

	histos1D_[ "HTPassing" ] = fileService->make< TH1D >( "HTPassing", "HT passing", 100, 0., 2000);
	histos1D_[ "HTPassing" ]->SetXTitle( "HT [GeV]" );

	histos1D_[ "HTEfficiency" ] = fileService->make< TH1D >( "HTEfficiency", "HT efficiency", 100, 0., 2000);
	histos1D_[ "HTEfficiency" ]->SetXTitle( "HT [GeV]" );
	histos1D_[ "HTEfficiency" ]->SetYTitle( "Efficiency" );

	histos1D_[ "ptDenom" ] = fileService->make< TH1D >( "ptDenom", "pt", 100, 0., 1000);
	histos1D_[ "ptDenom" ]->SetXTitle( "pt [GeV]" );

	histos1D_[ "ptPassing" ] = fileService->make< TH1D >( "ptPassing", "pt passing", 100, 0., 1000);
	histos1D_[ "ptPassing" ]->SetXTitle( "pt [GeV]" );

	histos1D_[ "ptEfficiency" ] = fileService->make< TH1D >( "ptEfficiency", "pt efficiency", 100, 0., 1000);
	histos1D_[ "ptEfficiency" ]->SetXTitle( "pt [GeV]" );
	histos1D_[ "ptEfficiency" ]->SetYTitle( "Efficiency" );

	histos1D_[ "numPV" ] = fileService->make< TH1D >( "numPV", "Number of Primary Vertex", 100, 0., 100);

	histos1D_[ "hltJetMass" ] = fileService->make< TH1D >( "hltJetMass", "Jet mass ", 40, 0., 400);

	histos1D_[ "patJetMass" ] = fileService->make< TH1D >( "patJetMass", "Jet mass ", 40, 0., 400);


	///// Sumw2 all the histos
	for( auto const& histo : histos1D_ ) histos1D_[ histo.first ]->Sumw2();
	for( auto const& histo : histos2D_ ) histos2D_[ histo.first ]->Sumw2();

}

// ------------ method called once each job just after ending the event loop  ------------
void TriggerEfficiencyPlots::endJob() {

/*    histos2D_[ "jetMassHT2Defficiency" ]->Divide(histos2D_[ "jetMassHTPassing" ],histos2D_[ "jetMassHTDenom" ],1,1,"B");
    
    histos2D_[ "jetMassPt2Defficiency" ]->Divide(histos2D_[ "jetMassPtPassing" ],histos2D_[ "jetMassPtDenom" ],1,1,"B");
    histos1D_[ "jetMassEfficiency" ]->Divide(histos1D_[ "jetMassPassing" ], histos1D_[ "jetMassDenom" ], 1,1,"B");

    histos1D_[ "HTEfficiency" ]->Divide(histos1D_[ "HTPassing" ], histos1D_[ "HTDenom" ], 1,1,"B");

    histos1D_[ "ptEfficiency" ]->Divide(histos1D_[ "ptPassing" ], histos1D_[ "ptDenom" ], 1,1,"B");
*/

}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void TriggerEfficiencyPlots::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	desc.add<edm::InputTag>("triggerResults",edm::InputTag("TriggerResults::HLT"));
	desc.add<edm::InputTag>("recoJets",edm::InputTag("ak4PFJetsCHS"));
	desc.add<edm::InputTag>("primaryVertex",edm::InputTag("offlinePrimaryVertices"));
	desc.add<std::string>("triggerPath", "HLT_PFHT900_v6");
	desc.add<double>("MinHT", 0.0);
	desc.add<double>("MinPt", 200.0);
	desc.add<double>("MinMass", 0.0);
	descriptions.add("TriggerEfficiencyPlots",desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerEfficiencyPlots);
