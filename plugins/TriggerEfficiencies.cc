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
#include "DataFormats/JetReco/interface/GenJet.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <fstream>

#include "PUHLT/PUmitigationatHLT/interface/CommonVariablesStructure.h"

using namespace edm;
using namespace std;
using namespace reco;
using namespace pat;

class TriggerEfficiencies : public EDAnalyzer {

	public:
		explicit TriggerEfficiencies(const ParameterSet&);
      		static void fillDescriptions(ConfigurationDescriptions & descriptions);
		~TriggerEfficiencies() {}

	private:
		virtual void analyze(const Event&, const EventSetup&) override;
      		virtual void beginJob() override;

	EDGetTokenT<TriggerResults> triggerBits_;
	//EDGetTokenT<trigger::TriggerFilterObjectWithRefs> triggerObjects_;
	EDGetTokenT<PFJetCollection> triggerObjects_;
	EDGetTokenT<PFJetCollection> recoJetToken_;
	EDGetTokenT<PFJetCollection> patJetToken_;
        EDGetTokenT<vector<reco::GenJet>> genJetToken_;
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

TriggerEfficiencies::TriggerEfficiencies(const ParameterSet& iConfig):
	triggerBits_(consumes<TriggerResults>(iConfig.getParameter<InputTag>("bits"))),
	//triggerObjects_(consumes<trigger::TriggerFilterObjectWithRef>(iConfig.getParameter<InputTag>("objects"))),
	triggerObjects_(consumes<PFJetCollection>(iConfig.getParameter<InputTag>("objects"))),
	recoJetToken_(consumes<PFJetCollection>(iConfig.getParameter<InputTag>("recoJets"))),
	patJetToken_(consumes<PFJetCollection>(iConfig.getParameter<InputTag>("patJets"))),
	genJetToken_(consumes<vector<reco::GenJet>>(iConfig.getParameter<InputTag>("genJets")))

{
	baseTrigger_ = iConfig.getParameter<string>("baseTrigger");
	triggerPass_ = iConfig.getParameter<vector<string>>("triggerPass");
	triggerOverlap_ = iConfig.getParameter<vector<int>>("triggerOverlap");
	triggerOverlapBase_ = iConfig.getParameter<vector<int>>("triggerOverlapBase");
	recojetPt_ = iConfig.getParameter<double>("recojetPt");
	AK8jets_ = iConfig.getParameter<bool>("AK8jets");
	DEBUG_ = iConfig.getParameter<bool>("DEBUG");
}

void TriggerEfficiencies::analyze(const Event& iEvent, const EventSetup& iSetup) {

	Handle<TriggerResults> triggerBits;
	//Handle<trigger::TriggerFilterObjectWithRefs> triggerObjects;
	Handle<PFJetCollection> triggerObjects;
	Handle<PFJetCollection> recojets;
	Handle<PFJetCollection> patjets;
	Handle<vector<reco::GenJet>> genjets;

	iEvent.getByToken(triggerBits_, triggerBits);
	iEvent.getByToken(triggerObjects_, triggerObjects);
	iEvent.getByToken(recoJetToken_, recojets);
	iEvent.getByToken(patJetToken_, patjets);
	iEvent.getByToken(genJetToken_, genjets);

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
		double hltHTpt20 = 0;
		int numHLTJetspt20 = 0;
		double hltHTpt30 = 0;
		int numHLTJetspt30 = 0;
		double hltHTpt40 = 0;
		int numHLTJetspt40 = 0;
		double hltHTpt50 = 0;
		int numHLTJetspt50 = 0;

		double hltHT_gen=0;
		double hltHT_trigger=0;
        
		for ( auto const& triggerJet : *triggerObjects) {

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

	   //response pt
		float mindr=10000;
                int dummyInd = -1;
                for (size_t i=0; i< genjets->size(); i++) {
                    float dr = deltaR(triggerJet, (*genjets)[i]);
                    if (dr < mindr){
                        mindr=dr;
                        if (mindr<0.2){ /// 0.3 is ok, but just to make sure
                            dummyInd=i;
                        }
                    }
                }
                if (dummyInd>-1){
                    const reco::GenJet &matchGenJet = (*genjets)[dummyInd];
                 
		    histos1D_["response_1D"]->Fill(triggerJet.pt()/matchGenJet.pt());
		    histos2D_["response_offline"]->Fill(triggerJet.pt(),triggerJet.pt()/matchGenJet.pt());
		    histos2D_[ "response" ]->Fill( matchGenJet.pt(),triggerJet.pt()/matchGenJet.pt() );

		    hltHT_gen+=matchGenJet.pt();
		    hltHT_trigger+=triggerJet.pt();
		    
                }

	    

	       if( triggerJet.pt() < 20 ) continue;
            hltHTpt20 += triggerJet.pt();
            numHLTJetspt20+=1;
            histos1D_[ "hltJetPt_pt20" ]->Fill( triggerJet.pt() );
            histos1D_[ "hltJetEta_pt20" ]->Fill( triggerJet.eta() );

	      if( triggerJet.pt() < 30 ) continue;
            hltHTpt30 += triggerJet.pt();
            numHLTJetspt30+=1;
            histos1D_[ "hltJetPt_pt30" ]->Fill( triggerJet.pt() );
            histos1D_[ "hltJetEta_pt30" ]->Fill( triggerJet.eta() );

	      if( triggerJet.pt() < 40 ) continue;
            hltHTpt40 += triggerJet.pt();
            numHLTJetspt40+=1;
            histos1D_[ "hltJetPt_pt40" ]->Fill( triggerJet.pt() );
            histos1D_[ "hltJetEta_pt40" ]->Fill( triggerJet.eta() );

	      if( triggerJet.pt() < 50 ) continue;
            hltHTpt50 += triggerJet.pt();
            numHLTJetspt50+=1;
            histos1D_[ "hltJetPt_pt50" ]->Fill( triggerJet.pt() );
            histos1D_[ "hltJetEta_pt50" ]->Fill( triggerJet.eta() );

        }
	histos1D_["hltresponseHT"]->Fill(hltHT_trigger/hltHT_gen);
        histos2D_[ "hltresponseHT_gen" ]->Fill( hltHT_gen, hltHT_trigger/hltHT_gen );
	histos2D_[ "hltresponseHT_trigger" ]->Fill( hltHT_trigger, hltHT_trigger/hltHT_gen );
        //histos2D_[ "hltJetPtvsMass" ]->Fill( hltPt, hltMass );
	histos1D_[ "hltJetHT" ]->Fill( hltHT );
        histos1D_[ "hltnumJets" ]->Fill( numHLTJets );
        histos1D_[ "hltJetHT_pt10" ]->Fill( hltHTpt10 );
        histos1D_[ "hltnumJets_pt10" ]->Fill( numHLTJetspt10 );
	histos1D_[ "hltJetHT_pt20" ]->Fill( hltHTpt20 );
        histos1D_[ "hltnumJets_pt20" ]->Fill( numHLTJetspt20 );
	histos1D_[ "hltJetHT_pt30" ]->Fill( hltHTpt30 );
        histos1D_[ "hltnumJets_pt30" ]->Fill( numHLTJetspt30 );
	histos1D_[ "hltJetHT_pt40" ]->Fill( hltHTpt40 );
        histos1D_[ "hltnumJets_pt40" ]->Fill( numHLTJetspt40 );
	histos1D_[ "hltJetHT_pt50" ]->Fill( hltHTpt50 );
        histos1D_[ "hltnumJets_pt50" ]->Fill( numHLTJetspt50 );
	    
        if ( baseTrigger || ORTriggers ) { // checking if both triggers passed

            /////////////////////////////////////////////////////////////////////////////////
            /// This is for recoJets
	    double HTpt10=0;
	    double HTpt20=0;
	    double HTpt30=0;
	    double HTpt40=0;
	    double HTpt50=0;

	    double HT_gen=0;
	    double HT_reco=0;
            //int k = 0;
            for (const reco::Jet &recojet : *recojets) {

	      if( recojet.pt() < recojetPt_ ) continue; 
                if( TMath::Abs(recojet.eta()) > 2.5 ) continue;
                if (DEBUG_) LogWarning("recojets") << recojet.pt();
		
		//basic case where recojetPt_=10
                HTpt10 += recojet.pt();

                //response pt
		float mindr=10000;
                int dummyInd = -1;
                for (size_t i=0; i< genjets->size(); i++) {
                    float dr = deltaR(recojet, (*genjets)[i]);
                    if (dr < mindr){
                        mindr=dr;
                        if (mindr<0.2){ /// 0.3 is ok, but just to make sure
                            dummyInd=i;
                        }
                    }
                }
                if (dummyInd>-1){
                    const reco::GenJet &matchGenJet = (*genjets)[dummyInd];
		    histos1D_["response_1D_reco"]->Fill(recojet.pt()/matchGenJet.pt());
		    histos2D_[ "response_reco" ]->Fill( matchGenJet.pt(),recojet.pt()/matchGenJet.pt() );

		    HT_gen+=matchGenJet.pt();
		    HT_reco+=recojet.pt();
		    
                }
		if (dummyInd==-1){ //recojet=pileupjet
                    
		  float mindr=10000;
		  int pu_dummyInd=-1;
		  for (size_t i=0; i< triggerObjects->size(); i++) {
                    float dr = deltaR(recojet, (*triggerObjects)[i]);
                    if (dr < mindr){
                        mindr=dr;
                        if (mindr<0.2){ /// 0.3 is ok, but just to make sure
                            pu_dummyInd=i;
                        }
                    }
		  }
		  const reco::Jet &matchTriggerJet = (*triggerObjects)[pu_dummyInd];
		  histos2D_[ "response_pileup" ]->Fill( recojet.pt(),matchTriggerJet.pt()/recojet.pt() );
		    
                }

		

		if(recojet.pt()<20) continue;
		HTpt20+=recojet.pt();

		if(recojet.pt()<30) continue;
		HTpt30+=recojet.pt();

		if(recojet.pt()<40) continue;
		HTpt40+=recojet.pt();

		if(recojet.pt()<50) continue;
		HTpt50+=recojet.pt();
		

                //if (++k==1){
		//histos1D_[ "jet1Pt" ]->Fill( recojet.pt() );

                    // this will help later, but good check for now.
                    //if ( baseTrigger ) {
		//histos1D_[ "jet1PtDenom" ]->Fill( recojet.pt() );
		//if ( ORTriggers ) histos1D_[ "jet1PtPassing" ]->Fill( recojet.pt() );
		//}
                //}

            }
	    histos1D_["responseHT"]->Fill(HT_reco/HT_gen);
	    histos2D_[ "responseHT_gen" ]->Fill( HT_gen, HT_reco/HT_gen );
	    histos2D_[ "responseHT_reco" ]->Fill( HT_reco, HT_reco/HT_gen );
	    
	    histos1D_[ "JetHT_pt10" ]->Fill( HTpt10 );
	    histos1D_[ "JetHT_pt20" ]->Fill( HTpt20 );
	    histos1D_[ "JetHT_pt30" ]->Fill( HTpt30 );
	    histos1D_[ "JetHT_pt40" ]->Fill( HTpt40 );
	    histos1D_[ "JetHT_pt50" ]->Fill( HTpt50 );
            
            if ( baseTrigger ) {
                histos1D_[ "HTDenom_pt10" ]->Fill( HTpt10 );
		histos1D_[ "HTDenom_pt20" ]->Fill( HTpt20 );
		histos1D_[ "HTDenom_pt30" ]->Fill( HTpt30 );
		histos1D_[ "HTDenom_pt40" ]->Fill( HTpt40 );
		histos1D_[ "HTDenom_pt50" ]->Fill( HTpt50 );
		
		
                //if ( ORTriggers ) histos1D_[ "HTPassing" ]->Fill( HT );
                //if ( hltHT > 700 ) histos1D_[ "HTPassingHT700" ]->Fill( HT ); //since recojetpt_=10, there is no offline HTpt0
                //if ( hltHT > 800 ) histos1D_[ "HTPassingHT800" ]->Fill( HT );
                //if ( hltHT > 850 ) histos1D_[ "HTPassingHT850" ]->Fill( HT );
                //if ( hltHT > 900 ) histos1D_[ "HTPassingHT900" ]->Fill( HT );
                //if ( hltHT > 950 ) histos1D_[ "HTPassingHT950" ]->Fill( HT );
                //if ( hltHT > 1000 ) histos1D_[ "HTPassingHT1000" ]->Fill( HT );
                //if ( hltHT > 1050 ) histos1D_[ "HTPassingHT1050" ]->Fill( HT );

                /// for HT with pt >10
                if ( hltHTpt10 > 800 ) histos1D_[ "HTPassingHT800pt10" ]->Fill( HTpt10 );
                if ( hltHTpt10 > 850 ) histos1D_[ "HTPassingHT850pt10" ]->Fill( HTpt10 );
                if ( hltHTpt10 > 900 ) histos1D_[ "HTPassingHT900pt10" ]->Fill( HTpt10 );
                if ( hltHTpt10 > 950 ) histos1D_[ "HTPassingHT950pt10" ]->Fill( HTpt10 );
                if ( hltHTpt10 > 1000 ) histos1D_[ "HTPassingHT1000pt10" ]->Fill( HTpt10 );
                if ( hltHTpt10 > 1050 ) histos1D_[ "HTPassingHT1050pt10" ]->Fill( HTpt10 );
                if ( hltHTpt10 > 1100 ) histos1D_[ "HTPassingHT1100pt10" ]->Fill( HTpt10 );

		 /// for HT with pt >20
                if ( hltHTpt20 > 800 ) histos1D_[ "HTPassingHT800pt20" ]->Fill( HTpt20 );
                if ( hltHTpt20 > 850 ) histos1D_[ "HTPassingHT850pt20" ]->Fill( HTpt20 );
                if ( hltHTpt20 > 900 ) histos1D_[ "HTPassingHT900pt20" ]->Fill( HTpt20 );
                if ( hltHTpt20 > 950 ) histos1D_[ "HTPassingHT950pt20" ]->Fill( HTpt20 );
                if ( hltHTpt20 > 1000 ) histos1D_[ "HTPassingHT1000pt20" ]->Fill( HTpt20 );
                if ( hltHTpt20 > 1050 ) histos1D_[ "HTPassingHT1050pt20" ]->Fill( HTpt20 );
                if ( hltHTpt20 > 1100 ) histos1D_[ "HTPassingHT1100pt20" ]->Fill( HTpt20 );

		 /// for HT with pt >30
                if ( hltHTpt30 > 800 ) histos1D_[ "HTPassingHT800pt30" ]->Fill( HTpt30 );
                if ( hltHTpt30 > 850 ) histos1D_[ "HTPassingHT850pt30" ]->Fill( HTpt30 );
                if ( hltHTpt30 > 900 ) histos1D_[ "HTPassingHT900pt30" ]->Fill( HTpt30 );
                if ( hltHTpt30 > 950 ) histos1D_[ "HTPassingHT950pt30" ]->Fill( HTpt30 );
                if ( hltHTpt30 > 1000 ) histos1D_[ "HTPassingHT1000pt30" ]->Fill( HTpt30 );
                if ( hltHTpt30 > 1050 ) histos1D_[ "HTPassingHT1050pt30" ]->Fill( HTpt30 );
                if ( hltHTpt30 > 1100 ) histos1D_[ "HTPassingHT1100pt30" ]->Fill( HTpt30 );
		
		 /// for HT with pt >40
                if ( hltHTpt40 > 800 ) histos1D_[ "HTPassingHT800pt40" ]->Fill( HTpt40 );
                if ( hltHTpt40 > 850 ) histos1D_[ "HTPassingHT850pt40" ]->Fill( HTpt40 );
                if ( hltHTpt40 > 900 ) histos1D_[ "HTPassingHT900pt40" ]->Fill( HTpt40 );
                if ( hltHTpt40 > 950 ) histos1D_[ "HTPassingHT950pt40" ]->Fill( HTpt40 );
                if ( hltHTpt40 > 1000 ) histos1D_[ "HTPassingHT1000pt40" ]->Fill( HTpt40 );
                if ( hltHTpt40 > 1050 ) histos1D_[ "HTPassingHT1050pt40" ]->Fill( HTpt40 );
                if ( hltHTpt40 > 1100 ) histos1D_[ "HTPassingHT1100pt40" ]->Fill( HTpt40 );
		
		 /// for HT with pt >50
                if ( hltHTpt50 > 800 ) histos1D_[ "HTPassingHT800pt50" ]->Fill( HTpt50 );
                if ( hltHTpt50 > 850 ) histos1D_[ "HTPassingHT850pt50" ]->Fill( HTpt50 );
                if ( hltHTpt50 > 900 ) histos1D_[ "HTPassingHT900pt50" ]->Fill( HTpt50 );
                if ( hltHTpt50 > 950 ) histos1D_[ "HTPassingHT950pt50" ]->Fill( HTpt50 );
                if ( hltHTpt50 > 1000 ) histos1D_[ "HTPassingHT1000pt50" ]->Fill( HTpt50 );
                if ( hltHTpt50 > 1050 ) histos1D_[ "HTPassingHT1050pt50" ]->Fill( HTpt50 );
                if ( hltHTpt50 > 1100 ) histos1D_[ "HTPassingHT1100pt50" ]->Fill( HTpt50 );
            }
            /////////////////////////////////////////////////////////////////////////////////

            /////////////////////////////////////////////////////////////////////////////////
            /// This is for patJets
            double puppiHTpt10 = 0;
	    double puppiHTpt20 = 0;
	    double puppiHTpt30 = 0;
	    double puppiHTpt40 = 0;
	    double puppiHTpt50 = 0;

	    double HT_gen_pat=0;
	    double HT_pat=0;
	    
            //int kk = 0;
            for (const reco::Jet &patjet : *patjets) {

                if( patjet.pt() < recojetPt_ ) continue;
                if( TMath::Abs(patjet.eta()) > 2.5 ) continue;
		
	       
                if (DEBUG_) LogWarning("patjets") << patjet.pt();
                puppiHTpt10 += patjet.pt();


		//response pt
		float mindr=10000;
                int dummyInd = -1;
                for (size_t i=0; i< genjets->size(); i++) {
                    float dr = deltaR(patjet, (*genjets)[i]);
                    if (dr < mindr){
                        mindr=dr;
                        if (mindr<0.2){ /// 0.3 is ok, but just to make sure
                            dummyInd=i;
                        }
                    }
                }
                if (dummyInd>-1){
                    const reco::GenJet &matchGenJet = (*genjets)[dummyInd];
                    
		    histos1D_["response_1D_pat"]->Fill(patjet.pt()/matchGenJet.pt());
		    histos2D_[ "response_pat" ]->Fill( matchGenJet.pt(),patjet.pt()/matchGenJet.pt() );

		    HT_gen_pat+=matchGenJet.pt();
		    HT_pat+=patjet.pt();
		    
                }

		if (dummyInd==-1){ //patjet=pileupjet
                    
		  float mindr=10000;
		  int pu_dummyInd=-1;
		  for (size_t i=0; i< triggerObjects->size(); i++) {
                    float dr = deltaR(patjet, (*triggerObjects)[i]);
                    if (dr < mindr){
                        mindr=dr;
                        if (mindr<0.2){ /// 0.3 is ok, but just to make sure
                            pu_dummyInd=i;
                        }
                    }
		  }
		  const reco::Jet &matchTriggerJet = (*triggerObjects)[pu_dummyInd];
		  histos2D_[ "response_pileup_pat" ]->Fill( patjet.pt(),matchTriggerJet.pt()/patjet.pt() );
		    
                }


                //if (++kk==1){
		//histos1D_[ "puppijet1Pt" ]->Fill( patjet.pt() );

                    // this will help later, but good check for now.
                    //if ( baseTrigger ) {
		// histos1D_[ "puppijet1PtDenom" ]->Fill( patjet.pt() );
		//      if ( ORTriggers ) histos1D_[ "puppijet1PtPassing" ]->Fill( patjet.pt() );
		//  }
                //}

	    
		if(patjet.pt()<20) continue;
		puppiHTpt20+=patjet.pt();

		if(patjet.pt()<30) continue;
		puppiHTpt30+=patjet.pt();

		if(patjet.pt()<40) continue;
		puppiHTpt40+=patjet.pt();

		if(patjet.pt()<50) continue;
		puppiHTpt50+=patjet.pt();

		

            }
	    histos1D_["responseHT_pat"]->Fill(HT_pat/HT_gen_pat);
	    histos2D_[ "responseHT_gen_pat" ]->Fill( HT_gen_pat, HT_pat/HT_gen_pat );
	    histos2D_[ "responseHT_pat" ]->Fill( HT_pat, HT_pat/HT_gen_pat );

            histos1D_[ "puppiJetHT_pt10" ]->Fill( puppiHTpt10 );
	    histos1D_[ "puppiJetHT_pt20" ]->Fill( puppiHTpt20 );
	    histos1D_[ "puppiJetHT_pt30" ]->Fill( puppiHTpt30 );
	    histos1D_[ "puppiJetHT_pt40" ]->Fill( puppiHTpt40 );
	    histos1D_[ "puppiJetHT_pt50" ]->Fill( puppiHTpt50 );
            
            if ( baseTrigger ) {
                histos1D_[ "puppiHTDenom_pt10" ]->Fill( puppiHTpt10 );
		histos1D_[ "puppiHTDenom_pt20" ]->Fill( puppiHTpt20 );
		histos1D_[ "puppiHTDenom_pt30" ]->Fill( puppiHTpt30 );
		histos1D_[ "puppiHTDenom_pt40" ]->Fill( puppiHTpt40 );
		histos1D_[ "puppiHTDenom_pt50" ]->Fill( puppiHTpt50 );

                //if ( ORTriggers ) histos1D_[ "puppiHTPassing" ]->Fill( puppiHT );
		  //if ( hltHT > 700 ) histos1D_[ "puppiHTPassingHT700" ]->Fill( puppiHT );
		  //if ( hltHT > 800 ) histos1D_[ "puppiHTPassingHT800" ]->Fill( puppiHT );
		  //if ( hltHT > 850 ) histos1D_[ "puppiHTPassingHT850" ]->Fill( puppiHT );
		  //if ( hltHT > 900 ) histos1D_[ "puppiHTPassingHT900" ]->Fill( puppiHT );
		  //if ( hltHT > 950 ) histos1D_[ "puppiHTPassingHT950" ]->Fill( puppiHT );
		  //if ( hltHT > 1000 ) histos1D_[ "puppiHTPassingHT1000" ]->Fill( puppiHT );
		  //if ( hltHT > 1050 ) histos1D_[ "puppiHTPassingHT1050" ]->Fill( puppiHT );

                /// for puppiHT with pt >10
                if ( hltHTpt10 > 800 ) histos1D_[ "puppiHTPassingHT800pt10" ]->Fill( puppiHTpt10 );
                if ( hltHTpt10 > 850 ) histos1D_[ "puppiHTPassingHT850pt10" ]->Fill( puppiHTpt10 );
                if ( hltHTpt10 > 900 ) histos1D_[ "puppiHTPassingHT900pt10" ]->Fill( puppiHTpt10 );
                if ( hltHTpt10 > 950 ) histos1D_[ "puppiHTPassingHT950pt10" ]->Fill( puppiHTpt10 );
                if ( hltHTpt10 > 1000 ) histos1D_[ "puppiHTPassingHT1000pt10" ]->Fill( puppiHTpt10 );
                if ( hltHTpt10 > 1050 ) histos1D_[ "puppiHTPassingHT1050pt10" ]->Fill( puppiHTpt10 );
                if ( hltHTpt10 > 1100 ) histos1D_[ "puppiHTPassingHT1100pt10" ]->Fill( puppiHTpt10 );

		/// for puppiHT with pt >20
                if ( hltHTpt20 > 800 ) histos1D_[ "puppiHTPassingHT800pt20" ]->Fill( puppiHTpt20 );
                if ( hltHTpt20 > 850 ) histos1D_[ "puppiHTPassingHT850pt20" ]->Fill( puppiHTpt20 );
                if ( hltHTpt20 > 900 ) histos1D_[ "puppiHTPassingHT900pt20" ]->Fill( puppiHTpt20 );
                if ( hltHTpt20 > 950 ) histos1D_[ "puppiHTPassingHT950pt20" ]->Fill( puppiHTpt20 );
                if ( hltHTpt20 > 1000 ) histos1D_[ "puppiHTPassingHT1000pt20" ]->Fill( puppiHTpt20 );
                if ( hltHTpt20 > 1050 ) histos1D_[ "puppiHTPassingHT1050pt20" ]->Fill( puppiHTpt20 );
                if ( hltHTpt20 > 1100 ) histos1D_[ "puppiHTPassingHT1100pt20" ]->Fill( puppiHTpt20 );

		/// for puppiHT with pt >30
                if ( hltHTpt30 > 800 ) histos1D_[ "puppiHTPassingHT800pt30" ]->Fill( puppiHTpt30 );
                if ( hltHTpt30 > 850 ) histos1D_[ "puppiHTPassingHT850pt30" ]->Fill( puppiHTpt30 );
                if ( hltHTpt30 > 900 ) histos1D_[ "puppiHTPassingHT900pt30" ]->Fill( puppiHTpt30 );
                if ( hltHTpt30 > 950 ) histos1D_[ "puppiHTPassingHT950pt30" ]->Fill( puppiHTpt30 );
                if ( hltHTpt30 > 1000 ) histos1D_[ "puppiHTPassingHT1000pt30" ]->Fill( puppiHTpt30 );
                if ( hltHTpt30 > 1050 ) histos1D_[ "puppiHTPassingHT1050pt30" ]->Fill( puppiHTpt30 );
                if ( hltHTpt30 > 1100 ) histos1D_[ "puppiHTPassingHT1100pt30" ]->Fill( puppiHTpt30 );

		/// for puppiHT with pt >40
                if ( hltHTpt40 > 800 ) histos1D_[ "puppiHTPassingHT800pt40" ]->Fill( puppiHTpt40 );
                if ( hltHTpt40 > 850 ) histos1D_[ "puppiHTPassingHT850pt40" ]->Fill( puppiHTpt40 );
                if ( hltHTpt40 > 900 ) histos1D_[ "puppiHTPassingHT900pt40" ]->Fill( puppiHTpt40 );
                if ( hltHTpt40 > 950 ) histos1D_[ "puppiHTPassingHT950pt40" ]->Fill( puppiHTpt40 );
                if ( hltHTpt40 > 1000 ) histos1D_[ "puppiHTPassingHT1000pt40" ]->Fill( puppiHTpt40 );
                if ( hltHTpt40 > 1050 ) histos1D_[ "puppiHTPassingHT1050pt40" ]->Fill( puppiHTpt40 );
                if ( hltHTpt40 > 1100 ) histos1D_[ "puppiHTPassingHT1100pt40" ]->Fill( puppiHTpt40 );

		/// for puppiHT with pt >50
                if ( hltHTpt50 > 800 ) histos1D_[ "puppiHTPassingHT800pt50" ]->Fill( puppiHTpt50 );
                if ( hltHTpt50 > 850 ) histos1D_[ "puppiHTPassingHT850pt50" ]->Fill( puppiHTpt50 );
                if ( hltHTpt50 > 900 ) histos1D_[ "puppiHTPassingHT900pt50" ]->Fill( puppiHTpt50 );
                if ( hltHTpt50 > 950 ) histos1D_[ "puppiHTPassingHT950pt50" ]->Fill( puppiHTpt50 );
                if ( hltHTpt50 > 1000 ) histos1D_[ "puppiHTPassingHT1000pt50" ]->Fill( puppiHTpt50 );
                if ( hltHTpt50 > 1050 ) histos1D_[ "puppiHTPassingHT1050pt50" ]->Fill( puppiHTpt50 );
                if ( hltHTpt50 > 1100 ) histos1D_[ "puppiHTPassingHT1100pt50" ]->Fill( puppiHTpt50 );
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

void TriggerEfficiencies::beginJob() {

        //histos2D_[ "hltJetPtvsMass" ] = fs_->make< TH2D >( "hltJetPtvsMass", "hltJetPtvsMass", 2000, 0., 2000., 2000, 0., 2000. );
        
  histos2D_[ "response_reco" ] = fs_->make< TH2D >( "response_reco", "response_reco", 100, 0., 1000., 100, 0, 5 );
	histos2D_[ "response_pat" ] = fs_->make< TH2D >( "response_pat", "response_pat", 100, 0., 1000., 100, 0, 5 );
	histos2D_[ "response" ] = fs_->make< TH2D >( "response", "response", 100, 0., 1000., 100, 0, 5 );
	histos2D_[ "response_offline" ] = fs_->make< TH2D >( "response_offline", "response_offline", 100, 0., 1000., 100, 0, 5 );
	histos2D_[ "response_pileup" ] = fs_->make< TH2D >( "response_pileup", "response_pileup", 100, 0., 1000., 100, 0, 5 );
	histos2D_[ "response_pileup_pat" ] = fs_->make< TH2D >( "response_pileup_pat", "response_pileup_pat", 100, 0., 1000., 100, 0, 5 );
	
	histos1D_[ "response_1D" ] = fs_->make< TH1D >( "response_1D", "response_1D", 100, 0., 5. );
	histos1D_[ "response_1D_reco" ] = fs_->make< TH1D >( "response_1D_reco", "response_1D_reco", 100, 0., 5. );
       	histos1D_[ "response_1D_pat" ] = fs_->make< TH1D >( "response_1D_pat", "response_1D_pat", 100, 0., 5. );

	histos1D_[ "hltresponseHT" ] = fs_->make< TH1D >( "hltresponseHT", "hltresponseHT", 100, 0., 5. );
	histos1D_[ "responseHT" ] = fs_->make< TH1D >( "responseHT", "responseHT", 100, 0., 5. );
	histos1D_[ "responseHT_pat" ] = fs_->make< TH1D >( "responseHT_pat", "responseHT_pat", 100, 0., 5. );

	histos2D_[ "hltresponseHT_gen" ] = fs_->make< TH2D >( "hltresponseHT_gen", "hltresponseHT_gen", 150, 500, 2000., 100, 0, 5 );
	histos2D_[ "hltresponseHT_trigger" ] = fs_->make< TH2D >( "hltresponseHT_trigger", "hltresponseHT_trigger", 150, 500, 2000., 100, 0, 5 );
	histos2D_[ "responseHT_gen" ] = fs_->make< TH2D >( "responseHT_gen", "responseHT_gen", 150, 500, 2000., 100, 0, 5 );
	histos2D_[ "responseHT_reco" ] = fs_->make< TH2D >( "responseHT_reco", "responseHT_reco", 150, 500, 2000., 100, 0, 5 );
	histos2D_[ "responseHT_gen_pat" ] = fs_->make< TH2D >( "responseHT_gen_pat", "responseHT_gen_pat", 150, 500, 2000., 100, 0, 5 );
	histos2D_[ "responseHT_pat" ] = fs_->make< TH2D >( "responseHT_pat", "responseHT_pat", 150, 500, 2000., 100, 0, 5 );

	histos1D_[ "hltJetPt" ] = fs_->make< TH1D >( "hltJetPt", "hltJetPt", 2000, 0., 2000. );
	//histos1D_[ "hltJetMass" ] = fs_->make< TH1D >( "hltJetMass", "hltJetMass", 2000, 0., 2000. );
	histos1D_[ "hltJetEta" ] = fs_->make< TH1D >( "hltJetEta", "hltJetEta", 100, -5, 5 );
	histos1D_[ "hltJetHT" ] = fs_->make< TH1D >( "hltJetHT", "hltJetHT", 5000, 0., 5000. );
	histos1D_[ "hltnumJets" ] = fs_->make< TH1D >( "hltnumJets", "hltnumJets", 20, 0., 20. );

	histos1D_[ "hltJetPt_pt10" ] = fs_->make< TH1D >( "hltJetPt_pt10", "hltJetPt_pt10", 2000, 0., 2000. );
	histos1D_[ "hltJetEta_pt10" ] = fs_->make< TH1D >( "hltJetEta_pt10", "hltJetEta_pt10", 100, -5, 5 );
	histos1D_[ "hltJetHT_pt10" ] = fs_->make< TH1D >( "hltJetHT_pt10", "hltJetHT_pt10", 5000, 0., 5000. );
	histos1D_[ "hltnumJets_pt10" ] = fs_->make< TH1D >( "hltnumJets_pt10", "hltnumJets_pt10", 20, 0., 20. );
	
	histos1D_[ "hltJetPt_pt20" ] = fs_->make< TH1D >( "hltJetPt_pt20", "hltJetPt_pt20", 2000, 0., 2000. );
	histos1D_[ "hltJetEta_pt20" ] = fs_->make< TH1D >( "hltJetEta_pt20", "hltJetEta_pt20", 100, -5, 5 );
	histos1D_[ "hltJetHT_pt20" ] = fs_->make< TH1D >( "hltJetHT_pt20", "hltJetHT_pt20", 5000, 0., 5000. );
	histos1D_[ "hltnumJets_pt20" ] = fs_->make< TH1D >( "hltnumJets_pt20", "hltnumJets_pt20", 20, 0., 20. );

	histos1D_[ "hltJetPt_pt30" ] = fs_->make< TH1D >( "hltJetPt_pt30", "hltJetPt_pt30", 2000, 0., 2000. );
	histos1D_[ "hltJetEta_pt30" ] = fs_->make< TH1D >( "hltJetEta_pt30", "hltJetEta_pt30", 100, -5, 5 );
	histos1D_[ "hltJetHT_pt30" ] = fs_->make< TH1D >( "hltJetHT_pt30", "hltJetHT_pt30", 5000, 0., 5000. );
	histos1D_[ "hltnumJets_pt30" ] = fs_->make< TH1D >( "hltnumJets_pt30", "hltnumJets_pt30", 20, 0., 20. );

	histos1D_[ "hltJetPt_pt40" ] = fs_->make< TH1D >( "hltJetPt_pt40", "hltJetPt_pt40", 2000, 0., 2000. );
	histos1D_[ "hltJetEta_pt40" ] = fs_->make< TH1D >( "hltJetEta_pt40", "hltJetEta_pt40", 100, -5, 5 );
	histos1D_[ "hltJetHT_pt40" ] = fs_->make< TH1D >( "hltJetHT_pt40", "hltJetHT_pt40", 5000, 0., 5000. );
	histos1D_[ "hltnumJets_pt40" ] = fs_->make< TH1D >( "hltnumJets_pt40", "hltnumJets_pt40", 20, 0., 20. );

	histos1D_[ "hltJetPt_pt50" ] = fs_->make< TH1D >( "hltJetPt_pt50", "hltJetPt_pt50", 2000, 0., 2000. );
	histos1D_[ "hltJetEta_pt50" ] = fs_->make< TH1D >( "hltJetEta_pt50", "hltJetEta_pt50", 100, -5, 5 );
	histos1D_[ "hltJetHT_pt50" ] = fs_->make< TH1D >( "hltJetHT_pt50", "hltJetHT_pt50", 5000, 0., 5000. );
	histos1D_[ "hltnumJets_pt50" ] = fs_->make< TH1D >( "hltnumJets_pt50", "hltnumJets_pt50", 20, 0., 20. );

	

	//histos2D_[ "hltJetPtvsMass" ] = fs_->make< TH2D >( "hltJetPtvsMass", "hltJetPtvsMass", 2000, 0., 2000., 2000, 0., 2000. );

	//histos1D_[ "jet1Pt" ] = fs_->make< TH1D >( "jet1Pt", "jet1Pt", 1000, 0., 1000. );
	histos1D_[ "JetHT_pt10" ] = fs_->make< TH1D >( "JetHT_pt10", "JetHT_pt10", 100, 0., 2000. );
	histos1D_[ "JetHT_pt20" ] = fs_->make< TH1D >( "JetHT_pt20", "JetHT_pt20", 100, 0., 2000. );
	histos1D_[ "JetHT_pt30" ] = fs_->make< TH1D >( "JetHT_pt30", "JetHT_pt30", 100, 0., 2000. );
	histos1D_[ "JetHT_pt40" ] = fs_->make< TH1D >( "JetHT_pt40", "JetHT_pt40", 100, 0., 2000. );
	histos1D_[ "JetHT_pt50" ] = fs_->make< TH1D >( "JetHT_pt50", "JetHT_pt50", 100, 0., 2000. );


	//histos1D_[ "jet1PtDenom" ] = fs_->make< TH1D >( "jet1PtDenom", "jet1PtDenom", 1000, 0., 1000. );
	//histos1D_[ "jet1PtPassing" ] = fs_->make< TH1D >( "jet1PtPassing", "jet1PtPassing", 1000, 0., 1000. );
	//histos1D_[ "HTDenom" ] = fs_->make< TH1D >( "HTDenom", "HTDenom", 2000, 0., 2000. );
	//histos1D_[ "HTPassing" ] = fs_->make< TH1D >( "HTPassing", "HTPassing", 2000, 0., 2000. );
	//histos1D_[ "HTPassingHT700" ] = fs_->make< TH1D >( "HTPassingHT700", "HTPassingHT700", 2000, 0., 2000. );
	//histos1D_[ "HTPassingHT800" ] = fs_->make< TH1D >( "HTPassingHT800", "HTPassingHT800", 2000, 0., 2000. );
	//histos1D_[ "HTPassingHT850" ] = fs_->make< TH1D >( "HTPassingHT850", "HTPassingHT850", 2000, 0., 2000. );
	//histos1D_[ "HTPassingHT900" ] = fs_->make< TH1D >( "HTPassingHT900", "HTPassingHT900", 2000, 0., 2000. );
	//histos1D_[ "HTPassingHT950" ] = fs_->make< TH1D >( "HTPassingHT950", "HTPassingHT950", 2000, 0., 2000. );
	//histos1D_[ "HTPassingHT1000" ] = fs_->make< TH1D >( "HTPassingHT1000", "HTPassingHT1000", 2000, 0., 2000. );
	//histos1D_[ "HTPassingHT1050" ] = fs_->make< TH1D >( "HTPassingHT1050", "HTPassingHT1050", 2000, 0., 2000. );

	histos1D_[ "HTDenom_pt10" ] = fs_->make< TH1D >( "HTDenom_pt10", "HTDenom_pt10", 2000, 0., 2000. );
	histos1D_[ "HTDenom_pt20" ] = fs_->make< TH1D >( "HTDenom_pt20", "HTDenom_pt20", 2000, 0., 2000. );
	histos1D_[ "HTDenom_pt30" ] = fs_->make< TH1D >( "HTDenom_pt30", "HTDenom_pt30", 2000, 0., 2000. );
	histos1D_[ "HTDenom_pt40" ] = fs_->make< TH1D >( "HTDenom_pt40", "HTDenom_pt40", 2000, 0., 2000. );
	histos1D_[ "HTDenom_pt50" ] = fs_->make< TH1D >( "HTDenom_pt50", "HTDenom_pt50", 2000, 0., 2000. );

	histos1D_[ "HTPassingHT800pt10" ] = fs_->make< TH1D >( "HTPassingHT800pt10", "HTPassingHT800pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT850pt10" ] = fs_->make< TH1D >( "HTPassingHT850pt10", "HTPassingHT850pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT900pt10" ] = fs_->make< TH1D >( "HTPassingHT900pt10", "HTPassingHT900pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT950pt10" ] = fs_->make< TH1D >( "HTPassingHT950pt10", "HTPassingHT950pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1000pt10" ] = fs_->make< TH1D >( "HTPassingHT1000pt10", "HTPassingHT1000pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1050pt10" ] = fs_->make< TH1D >( "HTPassingHT1050pt10", "HTPassingHT1050pt10", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1100pt10" ] = fs_->make< TH1D >( "HTPassingHT1100pt10", "HTPassingHT1100pt10", 2000, 0., 2000. );

	histos1D_[ "HTPassingHT800pt20" ] = fs_->make< TH1D >( "HTPassingHT800pt20", "HTPassingHT800pt20", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT850pt20" ] = fs_->make< TH1D >( "HTPassingHT850pt20", "HTPassingHT850pt20", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT900pt20" ] = fs_->make< TH1D >( "HTPassingHT900pt20", "HTPassingHT900pt20", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT950pt20" ] = fs_->make< TH1D >( "HTPassingHT950pt20", "HTPassingHT950pt20", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1000pt20" ] = fs_->make< TH1D >( "HTPassingHT1000pt20", "HTPassingHT1000pt20", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1050pt20" ] = fs_->make< TH1D >( "HTPassingHT1050pt20", "HTPassingHT1050pt20", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1100pt20" ] = fs_->make< TH1D >( "HTPassingHT1100pt20", "HTPassingHT1100pt20", 2000, 0., 2000. );

	histos1D_[ "HTPassingHT800pt30" ] = fs_->make< TH1D >( "HTPassingHT800pt30", "HTPassingHT800pt30", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT850pt30" ] = fs_->make< TH1D >( "HTPassingHT850pt30", "HTPassingHT850pt30", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT900pt30" ] = fs_->make< TH1D >( "HTPassingHT900pt30", "HTPassingHT900pt30", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT950pt30" ] = fs_->make< TH1D >( "HTPassingHT950pt30", "HTPassingHT950pt30", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1000pt30" ] = fs_->make< TH1D >( "HTPassingHT1000pt30", "HTPassingHT1000pt30", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1050pt30" ] = fs_->make< TH1D >( "HTPassingHT1050pt30", "HTPassingHT1050pt30", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1100pt30" ] = fs_->make< TH1D >( "HTPassingHT1100pt30", "HTPassingHT1100pt30", 2000, 0., 2000. );

	histos1D_[ "HTPassingHT800pt40" ] = fs_->make< TH1D >( "HTPassingHT800pt40", "HTPassingHT800pt40", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT850pt40" ] = fs_->make< TH1D >( "HTPassingHT850pt40", "HTPassingHT850pt40", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT900pt40" ] = fs_->make< TH1D >( "HTPassingHT900pt40", "HTPassingHT900pt40", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT950pt40" ] = fs_->make< TH1D >( "HTPassingHT950pt40", "HTPassingHT950pt40", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1000pt40" ] = fs_->make< TH1D >( "HTPassingHT1000pt40", "HTPassingHT1000pt40", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1050pt40" ] = fs_->make< TH1D >( "HTPassingHT1050pt40", "HTPassingHT1050pt40", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1100pt40" ] = fs_->make< TH1D >( "HTPassingHT1100pt40", "HTPassingHT1100pt40", 2000, 0., 2000. );

	histos1D_[ "HTPassingHT800pt50" ] = fs_->make< TH1D >( "HTPassingHT800pt50", "HTPassingHT800pt50", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT850pt50" ] = fs_->make< TH1D >( "HTPassingHT850pt50", "HTPassingHT850pt50", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT900pt50" ] = fs_->make< TH1D >( "HTPassingHT900pt50", "HTPassingHT900pt50", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT950pt50" ] = fs_->make< TH1D >( "HTPassingHT950pt50", "HTPassingHT950pt50", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1000pt50" ] = fs_->make< TH1D >( "HTPassingHT1000pt50", "HTPassingHT1000pt50", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1050pt50" ] = fs_->make< TH1D >( "HTPassingHT1050pt50", "HTPassingHT1050pt50", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1100pt50" ] = fs_->make< TH1D >( "HTPassingHT1100pt50", "HTPassingHT1100pt50", 2000, 0., 2000. );

    /////////////////////////////////////////////////////////////
	//histos1D_[ "puppijet1Pt" ] = fs_->make< TH1D >( "puppijet1Pt", "puppijet1Pt", 1000, 0., 1000. );
	histos1D_[ "puppiJetHT_pt10" ] = fs_->make< TH1D >( "puppiJetHT_pt10", "puppiJetHT_pt10", 100, 0., 2000. );
	histos1D_[ "puppiJetHT_pt20" ] = fs_->make< TH1D >( "puppiJetHT_pt20", "puppiJetHT_pt20", 100, 0., 2000. );
	histos1D_[ "puppiJetHT_pt30" ] = fs_->make< TH1D >( "puppiJetHT_pt30", "puppiJetHT_pt30", 100, 0., 2000. );
	histos1D_[ "puppiJetHT_pt40" ] = fs_->make< TH1D >( "puppiJetHT_pt40", "puppiJetHT_pt40", 100, 0., 2000. );
	histos1D_[ "puppiJetHT_pt50" ] = fs_->make< TH1D >( "puppiJetHT_pt50", "puppiJetHT_pt50", 100, 0., 2000. );


	//histos1D_[ "puppijet1PtDenom" ] = fs_->make< TH1D >( "puppijet1PtDenom", "puppijet1PtDenom", 1000, 0., 1000. );
	//histos1D_[ "puppijet1PtPassing" ] = fs_->make< TH1D >( "puppijet1PtPassing", "puppijet1PtPassing", 1000, 0., 1000. );
	//histos1D_[ "puppiHTDenom" ] = fs_->make< TH1D >( "puppiHTDenom", "puppiHTDenom", 2000, 0., 2000. );
	//histos1D_[ "puppiHTPassing" ] = fs_->make< TH1D >( "puppiHTPassing", "puppiHTPassing", 2000, 0., 2000. );
	//histos1D_[ "puppiHTPassingHT700" ] = fs_->make< TH1D >( "puppiHTPassingHT700", "puppiHTPassingHT700", 2000, 0., 2000. );
	//histos1D_[ "puppiHTPassingHT800" ] = fs_->make< TH1D >( "puppiHTPassingHT800", "puppiHTPassingHT800", 2000, 0., 2000. );
	//histos1D_[ "puppiHTPassingHT850" ] = fs_->make< TH1D >( "puppiHTPassingHT850", "puppiHTPassingHT850", 2000, 0., 2000. );
	//histos1D_[ "puppiHTPassingHT900" ] = fs_->make< TH1D >( "puppiHTPassingHT900", "puppiHTPassingHT900", 2000, 0., 2000. );
	//histos1D_[ "puppiHTPassingHT950" ] = fs_->make< TH1D >( "puppiHTPassingHT950", "puppiHTPassingHT950", 2000, 0., 2000. );
	//histos1D_[ "puppiHTPassingHT1000" ] = fs_->make< TH1D >( "puppiHTPassingHT1000", "puppiHTPassingHT1000", 2000, 0., 2000. );
	//histos1D_[ "puppiHTPassingHT1050" ] = fs_->make< TH1D >( "puppiHTPassingHT1050", "puppiHTPassingHT1050", 2000, 0., 2000. );

	histos1D_[ "puppiHTDenom_pt10" ] = fs_->make< TH1D >( "puppiHTDenom_pt10", "puppiHTDenom_pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTDenom_pt20" ] = fs_->make< TH1D >( "puppiHTDenom_pt20", "puppiHTDenom_pt20", 2000, 0., 2000. );
	histos1D_[ "puppiHTDenom_pt30" ] = fs_->make< TH1D >( "puppiHTDenom_pt30", "puppiHTDenom_pt30", 2000, 0., 2000. );
	histos1D_[ "puppiHTDenom_pt40" ] = fs_->make< TH1D >( "puppiHTDenom_pt40", "puppiHTDenom_pt40", 2000, 0., 2000. );
	histos1D_[ "puppiHTDenom_pt50" ] = fs_->make< TH1D >( "puppiHTDenom_pt50", "puppiHTDenom_pt50", 2000, 0., 2000. );


	histos1D_[ "puppiHTPassingHT800pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT800pt10", "puppiHTPassingHT800pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT850pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT850pt10", "puppiHTPassingHT850pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT900pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT900pt10", "puppiHTPassingHT900pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT950pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT950pt10", "puppiHTPassingHT950pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1000pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT1000pt10", "puppiHTPassingHT1000pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1050pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT1050pt10", "puppiHTPassingHT1050pt10", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1100pt10" ] = fs_->make< TH1D >( "puppiHTPassingHT1100pt10", "puppiHTPassingHT1100pt10", 2000, 0., 2000. );

	histos1D_[ "puppiHTPassingHT800pt20" ] = fs_->make< TH1D >( "puppiHTPassingHT800pt20", "puppiHTPassingHT800pt20", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT850pt20" ] = fs_->make< TH1D >( "puppiHTPassingHT850pt20", "puppiHTPassingHT850pt20", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT900pt20" ] = fs_->make< TH1D >( "puppiHTPassingHT900pt20", "puppiHTPassingHT900pt20", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT950pt20" ] = fs_->make< TH1D >( "puppiHTPassingHT950pt20", "puppiHTPassingHT950pt20", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1000pt20" ] = fs_->make< TH1D >( "puppiHTPassingHT1000pt20", "puppiHTPassingHT1000pt20", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1050pt20" ] = fs_->make< TH1D >( "puppiHTPassingHT1050pt20", "puppiHTPassingHT1050pt20", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1100pt20" ] = fs_->make< TH1D >( "puppiHTPassingHT1100pt20", "puppiHTPassingHT1100pt20", 2000, 0., 2000. );

	histos1D_[ "puppiHTPassingHT800pt30" ] = fs_->make< TH1D >( "puppiHTPassingHT800pt30", "puppiHTPassingHT800pt30", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT850pt30" ] = fs_->make< TH1D >( "puppiHTPassingHT850pt30", "puppiHTPassingHT850pt30", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT900pt30" ] = fs_->make< TH1D >( "puppiHTPassingHT900pt30", "puppiHTPassingHT900pt30", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT950pt30" ] = fs_->make< TH1D >( "puppiHTPassingHT950pt30", "puppiHTPassingHT950pt30", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1000pt30" ] = fs_->make< TH1D >( "puppiHTPassingHT1000pt30", "puppiHTPassingHT1000pt30", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1050pt30" ] = fs_->make< TH1D >( "puppiHTPassingHT1050pt30", "puppiHTPassingHT1050pt30", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1100pt30" ] = fs_->make< TH1D >( "puppiHTPassingHT1100pt30", "puppiHTPassingHT1100pt30", 2000, 0., 2000. );

	histos1D_[ "puppiHTPassingHT800pt40" ] = fs_->make< TH1D >( "puppiHTPassingHT800pt40", "puppiHTPassingHT800pt40", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT850pt40" ] = fs_->make< TH1D >( "puppiHTPassingHT850pt40", "puppiHTPassingHT850pt40", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT900pt40" ] = fs_->make< TH1D >( "puppiHTPassingHT900pt40", "puppiHTPassingHT900pt40", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT950pt40" ] = fs_->make< TH1D >( "puppiHTPassingHT950pt40", "puppiHTPassingHT950pt40", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1000pt40" ] = fs_->make< TH1D >( "puppiHTPassingHT1000pt40", "puppiHTPassingHT1000pt40", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1050pt40" ] = fs_->make< TH1D >( "puppiHTPassingHT1050pt40", "puppiHTPassingHT1050pt40", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1100pt40" ] = fs_->make< TH1D >( "puppiHTPassingHT1100pt40", "puppiHTPassingHT1100pt40", 2000, 0., 2000. );

	histos1D_[ "puppiHTPassingHT800pt50" ] = fs_->make< TH1D >( "puppiHTPassingHT800pt50", "puppiHTPassingHT800pt50", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT850pt50" ] = fs_->make< TH1D >( "puppiHTPassingHT850pt50", "puppiHTPassingHT850pt50", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT900pt50" ] = fs_->make< TH1D >( "puppiHTPassingHT900pt50", "puppiHTPassingHT900pt50", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT950pt50" ] = fs_->make< TH1D >( "puppiHTPassingHT950pt50", "puppiHTPassingHT950pt50", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1000pt50" ] = fs_->make< TH1D >( "puppiHTPassingHT1000pt50", "puppiHTPassingHT1000pt50", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1050pt50" ] = fs_->make< TH1D >( "puppiHTPassingHT1050pt50", "puppiHTPassingHT1050pt50", 2000, 0., 2000. );
	histos1D_[ "puppiHTPassingHT1100pt50" ] = fs_->make< TH1D >( "puppiHTPassingHT1100pt50", "puppiHTPassingHT1100pt50", 2000, 0., 2000. );



	///// Sumw2 all the histos
	for( auto const& histo : histos1D_ ) histos1D_[ histo.first ]->Sumw2();
	for( auto const& histo : histos2D_ ) histos2D_[ histo.first ]->Sumw2();
}


void TriggerEfficiencies::fillDescriptions(ConfigurationDescriptions & descriptions) {

	ParameterSetDescription desc;
	desc.add<InputTag>("bits", 	InputTag("TriggerResults", "", "HLT2"));
	desc.add<InputTag>("objects", 	InputTag("slimmedPatTrigger"));
	desc.add<string>("baseTrigger", 	"HLT_PFHT800");
	desc.add<double>("recojetPt", 	50);
	desc.add<bool>("AK8jets", 	true);
	desc.add<bool>("DEBUG", 	false);
	desc.add<InputTag>("recoJets", 	InputTag("slimmedJetsAK8"));
	desc.add<InputTag>("patJets", 	InputTag("slimmedJetsAK8"));
	desc.add<InputTag>("genJets", 	InputTag("ak4GenJetsNoNu"));
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
DEFINE_FWK_MODULE(TriggerEfficiencies);
