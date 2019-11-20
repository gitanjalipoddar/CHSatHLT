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
	histos1D_[ "hltJetEta" ] = fs_->make< TH1D >( "hltJetEta", "hltJetEta", 100, -5, 5 );
	histos1D_[ "hltJetHT" ] = fs_->make< TH1D >( "hltJetHT", "hltJetHT", 5000, 0., 5000. );
	histos1D_[ "hltnumJets" ] = fs_->make< TH1D >( "hltnumJets", "hltnumJets", 20, 0., 20. );

	histos1D_[ "recoJetPt" ] = fs_->make< TH1D >( "recoJetPt", "recoJetPt", 2000, 0., 2000. );
	histos1D_[ "recoJetEta" ] = fs_->make< TH1D >( "recoJetEta", "recoJetEta", 100, -5, 5 );

	histos1D_[ "HTDenom" ] = fs_->make< TH1D >( "HTDenom", "HTDenom", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT800" ] = fs_->make< TH1D >( "HTPassingHT800", "HTPassingHT800", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT850" ] = fs_->make< TH1D >( "HTPassingHT850", "HTPassingHT850", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT900" ] = fs_->make< TH1D >( "HTPassingHT900", "HTPassingHT900", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT950" ] = fs_->make< TH1D >( "HTPassingHT950", "HTPassingHT950", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1000" ] = fs_->make< TH1D >( "HTPassingHT1000", "HTPassingHT1000", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1050" ] = fs_->make< TH1D >( "HTPassingHT1050", "HTPassingHT1050", 2000, 0., 2000. );
	histos1D_[ "HTPassingHT1100" ] = fs_->make< TH1D >( "HTPassingHT1100", "HTPassingHT1100", 2000, 0., 2000. );;

	histos1D_[ "lead_genPt_1D" ] = fs_->make< TH1D >( "lead_genPt_1D", "lead_genPt_1D", 2000, 0., 2000. );
	histos1D_[ "lead_recoPt_1D" ] = fs_->make< TH1D >( "lead_recoPt_1D", "lead_recoPt_1D", 2000, 0., 2000. );
	histos1D_[ "lead_hltPt_1D" ] = fs_->make< TH1D >( "lead_hltPt_1D", "lead_hltPt_1D", 2000, 0., 2000. );
	histos1D_[ "genHT_1D" ] = fs_->make< TH1D >( "genHT_1D", "genHT_1D", 2000, 0., 2000. );
	histos1D_[ "recoHT_1D" ] = fs_->make< TH1D >( "recoHT_1D", "recoHT_1D", 2000, 0., 2000. );
	histos1D_[ "hltHT_1D" ] = fs_->make< TH1D >( "hltHT_1D", "hltHT_1D", 2000, 0., 2000. );

	histos1D_[ "lead_recoGenPt_1D" ] = fs_->make< TH1D >( "lead_recoGenPt_1D", "lead_recoGenPt_1D", 40, 0., 10. );
	histos1D_[ "lead_hltGenPt_1D" ] = fs_->make< TH1D >( "lead_hltGenPt_1D", "lead_hltGenPt_1D", 40, 0., 10. );
	histos1D_[ "lead_hltRecoPt_1D" ] = fs_->make< TH1D >( "lead_hltRecoPt_1D", "lead_hltRecoPt_1D", 40, 0., 10. );
	histos1D_[ "recoGenHT_1D" ] = fs_->make< TH1D >( "recoGenHT_1D", "recoGenHT_1D", 40, 0., 10. );
	histos1D_[ "hltGenHT_1D" ] = fs_->make< TH1D >( "hltGenHT_1D", "hltGenHT_1D", 40, 0., 10. );
	histos1D_[ "hltRecoHT_1D" ] = fs_->make< TH1D >( "hltRecoHT_1D", "hltRecoHT_1D", 40, 0., 10. );
	
	histos2D_[ "lead_recoGenPt_gen" ] = fs_->make< TH2D >( "lead_recoGenPt_gen", "lead_recoGenPt_gen", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "lead_recoGenPt_reco" ] = fs_->make< TH2D >( "lead_recoGenPt_reco", "lead_recoGenPt_reco", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "lead_hltGenPt_gen" ] = fs_->make< TH2D >( "lead_hltGenPt_gen", "lead_hltGenPt_gen", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "lead_hltGenPt_reco" ] = fs_->make< TH2D >( "lead_hltGenPt_reco", "lead_hltGenPt_reco", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "lead_hltRecoPt_gen" ] = fs_->make< TH2D >( "lead_hltRecoPt_gen", "lead_hltRecoPt_gen", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "lead_hltRecoPt_reco" ] = fs_->make< TH2D >( "lead_hltRecoPt_reco", "lead_hltRecoPt_reco", 2000, 0., 2000., 40, 0., 10. );

	histos2D_[ "recoGenHT_gen" ] = fs_->make< TH2D >( "recoGenHT_gen", "recoGenHT_gen", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "recoGenHT_reco" ] = fs_->make< TH2D >( "recoGenHT_reco", "recoGenHT_reco", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "hltGenHT_gen" ] = fs_->make< TH2D >( "hltGenHT_gen", "hltGenHT_gen", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "hltGenHT_reco" ] = fs_->make< TH2D >( "hltGenHT_reco", "hltGenHT_reco", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "hltRecoHT_gen" ] = fs_->make< TH2D >( "hltRecoHT_gen", "hltRecoHT_gen", 2000, 0., 2000., 40, 0., 10. );
	histos2D_[ "hltRecoHT_reco" ] = fs_->make< TH2D >( "hltRecoHT_reco", "hltRecoHT_reco", 2000, 0., 2000., 40, 0., 10. );



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
