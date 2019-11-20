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
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "PUHLT/PUmitigationatHLT/interface/CommonVariablesStructure.h"

using namespace edm;
using namespace std;
using namespace reco;
using namespace pat;

class matchedJet {
    public:
        pat::Jet recoJet;
        reco::Jet hltJet;
};

class TriggerEfficiencies : public EDAnalyzer {

	public:
		explicit TriggerEfficiencies(const ParameterSet&);
      		static void fillDescriptions(ConfigurationDescriptions & descriptions);
		~TriggerEfficiencies() {}

	private:
		virtual void analyze(const Event&, const EventSetup&) override;
      		virtual void beginJob() override;

	EDGetTokenT<PFJetCollection> hltJets_;
	EDGetTokenT<std::vector<pat::Jet>> recoJetToken_;
	double recojetPt_;
	bool AK8jets_;
	bool DEBUG_;

	Service<TFileService> fs_;
	map< string, TH1D* > histos1D_;
	map< string, TH2D* > histos2D_;
};

TriggerEfficiencies::TriggerEfficiencies(const ParameterSet& iConfig):
	hltJets_(consumes<PFJetCollection>(iConfig.getParameter<InputTag>("hltJets"))),
	recoJetToken_(consumes<std::vector<pat::Jet>>(iConfig.getParameter<InputTag>("recoJets")))
{
	recojetPt_ = iConfig.getParameter<double>("recojetPt");
	AK8jets_ = iConfig.getParameter<bool>("AK8jets");
	DEBUG_ = iConfig.getParameter<bool>("DEBUG");
}

void TriggerEfficiencies::analyze(const Event& iEvent, const EventSetup& iSetup) {

	Handle<PFJetCollection> hltJets;
	//Handle<PFJetCollection> recojets;
	Handle<std::vector<pat::Jet>> recojets;

	iEvent.getByToken(hltJets_, hltJets);
	iEvent.getByToken(recoJetToken_, recojets);


    //for hlt jets
    double hltHT = 0;
    int numHLTJets = 0;
    
    for ( auto const& hltJet : *hltJets ) {
        if( hltJet.pt() < recoJetpt_) continue;
        if( TMath::Abs(hltJet.eta()) > 2.5 ) continue;
        
        hltHT += hltJet.pt();
        numHLTJets+=1;
        histos1D_[ "hltJetPt" ]->Fill( hltJet.pt() );
        histos1D_[ "hltJetEta" ]->Fill( hltJet.eta() );
    }
    histos1D_[ "hltJetHT" ]->Fill( hltHT );
    histos1D_[ "hltnumJets" ]->Fill( numHLTJets );

    //for recojets
    double HT=0;
    vector<matchedJet> matchJets;
    
    for ( auto const& hltJet : *hltJets ) {
      if (hltJet.pt()<recojetPt_) continue;
      if( TMath::Abs( hltJet.eta()) > 2.5 ) continue;

      double dummyMin= 9999;
      int minInd = -1;
      for (size_t i=0; i< recojets->size(); i++) {
            double deltaRrecohlt = deltaR( (*recojets)[i], hltJet );
            if ( deltaRrecohlt < dummyMin ) { 
                dummyMin = deltaRrecohlt;
                minInd = i;
            }
      }
      if (dummyMin<.2){
	//if( (*recojets)[minInd].pt()<recoJetPt_ ) continue;
	//if( TMath::Abs(*recojets)[minInd].eta()) > 2.5 ) continue;
            matchedJet tmpJet;
            tmpJet.hltJet = hltJet;
            tmpJet.recoJet = (*recojets)[minInd];
            matchJets.push_back(tmpJet);
	    
	    histos1D_[ "recoJetPt" ]->Fill( (*recojets)[minInd].pt() );
	    histos1D_[ "recoJetEta" ]->Fill( (*recojets)[minInd].eta() );
	    HT+=(*recojets)[minInd].pt();
        }
    }
    histos1D_[ "HTDenom" ]->Fill( HT );
    if ( hltHT > 800 ) histos1D_[ "HTPassingHT800" ]->Fill( HT );
    if ( hltHT > 850 ) histos1D_[ "HTPassingHT850" ]->Fill( HT );
    if ( hltHT > 900 ) histos1D_[ "HTPassingHT900" ]->Fill( HT );
    if ( hltHT > 950 ) histos1D_[ "HTPassingHT950" ]->Fill( HT );
    if ( hltHT > 1000 ) histos1D_[ "HTPassingHT1000" ]->Fill( HT );
    if ( hltHT > 1050 ) histos1D_[ "HTPassingHT1050" ]->Fill( HT );
    if ( hltHT > 1100 ) histos1D_[ "HTPassingHT1100" ]->Fill( HT );
    /////////////////////////////////////////////////////////////////////////////////

    
    // For response
    int m = 0;
    double mhltHT = 0;
    double mrecoHT = 0;
    double mgenHT = 0;
    for (const auto &mjet: matchJets) {

      bool ifGenJet = mjet.recoJet.genJet(); //checking recojet has matching genjet
        //bool ifhltJet = mjet.hltJet;
        if ( ifGenJet ) {
            mgenHT += mjet.recoJet.genJet()->pt();
            mhltHT += mjet.hltJet.pt();
            mrecoHT += mjet.recoJet.pt();
            //LogWarning("test") << mjet.recoJet.pt() << " " << mjet.recoJet.genJet()->pt() << " " << mjet.hltJet.pt();
          
	    if ((m++)==1){ //first jet is leading jet
	      histos1D_[ "lead_genPt_1D" ]->Fill( mjet.recoJet.genJet()->pt() );
	      histos1D_[ "lead_recoPt_1D" ]->Fill( mjet.recoJet.pt() );
	      histos1D_[ "lead_hltPt_1D" ]->Fill( mjet.hltJet.pt() );
                
	      double hltGenPt = mjet.hltJet.pt() / mjet.recoJet.genJet()->pt();
	      double recoGenPt = mjet.recoJet.pt() / mjet.recoJet.genJet()->pt();
	      double hltRecoPt = mjet.hltJet.pt() / mjet.recoJet.pt();
                
	      histos1D_[ "lead_recoGenPt_1D" ]->Fill( recoGenPt );
	      histos2D_[ "lead_recoGenPt_gen" ]->Fill( mjet.recoJet.genJet()->pt(), recoGenPt);
	      histos2D_[ "lead_recoGenPt_reco" ]->Fill( mjet.recoJet.pt(), recoGenPt );
           
	      histos1D_[ "lead_hltGenPt_1D" ]->Fill( hltGenPt );
	      histos2D_[ "lead_hltGenPt_gen" ]->Fill( mjet.recoJet.genJet()->pt(), hltGenPt );
	      histos2D_[ "lead_hltGenPt_reco" ]->Fill( mjet.recoJet.pt(), hltGenPt );

	      histos1D_[ "lead_hltRecoPt_1D" ]->Fill( hltRecoPt );
	      histos2D_[ "lead_hltRecoPt_gen" ]->Fill( mjet.recoJet.genJet()->pt(), hltRecoPt );
	      histos2D_[ "lead_hltRecoPt_reco" ]->Fill( mjet.recoJet.pt(), hltRecoPt );
            }
	}
    }

    histos1D_[ "genHT_1D" ]->Fill( mgenHT );
    histos1D_[ "recoHT_1D" ]->Fill( mrecoHT );
    histos1D_[ "hltHT_1D" ]->Fill( mhltHT );

    double hltGenHT = mhltHT/mgenHT;
    double recoGenHT = mrecoHT/mgenHT;
    double hltRecoHT = mhltHT/mrecoHT;

     histos1D_[ "recoGenHT_1D" ]->Fill( recoGenHT );
     histos2D_[ "recoGenHT_gen" ]->Fill( mgenHT, recoGenHT);
     histos2D_[ "recoGenHT_reco" ]->Fill( mrecoHT, recoGenHT );
           
     histos1D_[ "hltGenHT_1D" ]->Fill( hltGenHT );
     histos2D_[ "hltGenHT_gen" ]->Fill( mgenHT, hltGenHT );
     histos2D_[ "hltGenHT_reco" ]->Fill( mrecoHT, hltGenHT );

     histos1D_[ "hltRecoHT_1D" ]->Fill( hltRecoHT );
     histos2D_[ "hltRecoHT_gen" ]->Fill( mgenHT, hltRecoHT );
     histos2D_[ "hltRecoHT_reco" ]->Fill( mrecoHT, hltRecoHT);   
}

void TriggerEfficiencies::beginJob() {

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

	histos1D_[ "genjet1Pt" ] = fs_->make< TH1D >( "genjet1Pt", "genjet1Pt", 2000, 0., 2000. );
	histos1D_[ "recojet1Pt" ] = fs_->make< TH1D >( "recojet1Pt", "recojet1Pt", 2000, 0., 2000. );
	histos1D_[ "hltjet1Pt" ] = fs_->make< TH1D >( "hltjet1Pt", "hltjet1Pt", 2000, 0., 2000. );
	histos1D_[ "genjetHT" ] = fs_->make< TH1D >( "genjetHT", "genjetHT", 2000, 0., 2000. );
	histos1D_[ "recojetHT" ] = fs_->make< TH1D >( "recojetHT", "recojetHT", 2000, 0., 2000. );
	histos1D_[ "hltjetHT" ] = fs_->make< TH1D >( "hltjetHT", "hltjetHT", 2000, 0., 2000. );
	histos1D_[ "genrecojet1Ptreso" ] = fs_->make< TH1D >( "genrecojet1Ptreso", "genrecojet1Ptreso", 40, 0., 10. );
	histos2D_[ "genrecojet1PtresovsrecoPt" ] = fs_->make< TH2D >( "genrecojet1PtresovsrecoPt", "genrecojet1PtresovsrecoPt", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "genrecojet1PtresovsgenPt" ] = fs_->make< TH2D >( "genrecojet1PtresovsgenPt", "genrecojet1PtresovsgenPt", 2000, 0., 2000., 40, 0., 10. );
	histos1D_[ "genhltjet1Ptreso" ] = fs_->make< TH1D >( "genhltjet1Ptreso", "genhltjet1Ptreso", 40, 0., 10. );
	histos2D_[ "genhltjet1PtresovsrecoPt" ] = fs_->make< TH2D >( "genhltjet1PtresovsrecoPt", "genhltjet1PtresovsrecoPt", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "genhltjet1PtresovsgenPt" ] = fs_->make< TH2D >( "genhltjet1PtresovsgenPt", "genhltjet1PtresovsgenPt", 2000, 0., 2000., 40, 0., 10. );
	histos1D_[ "genrecojet1HTreso" ] = fs_->make< TH1D >( "genrecojet1HTreso", "genrecojet1HTreso", 40, 0., 10. );
	histos2D_[ "genrecojet1HTresovsrecoHT" ] = fs_->make< TH2D >( "genrecojet1HTresovsrecoHT", "genrecojet1HTresovsrecoHT", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "genrecojet1HTresovsgenHT" ] = fs_->make< TH2D >( "genrecojet1HTresovsgenHT", "genrecojet1HTresovsgenHT", 2000, 0., 2000., 40, 0., 10. );
	histos1D_[ "genhltjet1HTreso" ] = fs_->make< TH1D >( "genhltjet1HTreso", "genhltjet1HTreso", 40, 0., 10. );
	histos2D_[ "genhltjet1HTresovsrecoHT" ] = fs_->make< TH2D >( "genhltjet1HTresovsrecoHT", "genhltjet1HTresovsrecoHT", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "genhltjet1HTresovsgenHT" ] = fs_->make< TH2D >( "genhltjet1HTresovsgenHT", "genhltjet1HTresovsgenHT", 2000, 0., 2000., 40, 0., 10. );

	///// Sumw2 all the histos
	for( auto const& histo : histos1D_ ) histos1D_[ histo.first ]->Sumw2();
	for( auto const& histo : histos2D_ ) histos2D_[ histo.first ]->Sumw2();
}

void TriggerEfficiencies::fillDescriptions(ConfigurationDescriptions & descriptions) {

	ParameterSetDescription desc;
	desc.add<InputTag>("hltJets", 	InputTag("slimmedPatTrigger"));
	desc.add<double>("recojetPt", 	50);
	desc.add<bool>("AK8jets", 	true);
	desc.add<bool>("DEBUG", 	false);
	desc.add<InputTag>("recoJets", 	InputTag("slimmedJetsAK8"));
	descriptions.addDefault(desc);
}
      
//define this as a plug-in
DEFINE_FWK_MODULE(TriggerEfficiencies);
