// -*- C++ -*-
//
// Package:    PUHLT/PUmitigationatHLT
// Class:      vertexAnalyzer
//
/**\class vertexAnalyzer vertexAnalyzer.cc PUHLT/PUmitigationatHLT/plugins/vertexAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Alejandro Gomez Espinosa
//         Created:  Wed, 28 Sep 2019 12:09:45 GMT
//
//


// system include files
#include <memory>
#include <TH1.h>
#include <TH2.h>
#include <TLorentzVector.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


using namespace edm;
using namespace std;
using namespace reco;

class vertexAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {

   public:
      explicit vertexAnalyzer(const edm::ParameterSet&);
      ~vertexAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

    // ----------member data ---------------------------
	EDGetTokenT<vector<reco::Vertex>> hltVertexToken_;
	EDGetTokenT<vector<reco::Vertex>> offlineVertexToken_;
    Service<TFileService> fs_;
    map< string, TH1D* > histos1D_;
    map< string, TH2D* > histos2D_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
vertexAnalyzer::vertexAnalyzer(const edm::ParameterSet& iConfig)
 :
  hltVertexToken_(consumes<vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("hltVertex"))),
  offlineVertexToken_(consumes<vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("offlineVertex")))

{
   //now do what ever initialization is needed

}


vertexAnalyzer::~vertexAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void vertexAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

    VertexCollection HLTVertexCollection;
    Handle<vector<reco::Vertex>> hltVertexHandle;
    bool foundHLTVertexColl = iEvent.getByToken(hltVertexToken_, hltVertexHandle);
    if( foundHLTVertexColl ) HLTVertexCollection = *hltVertexHandle;

    VertexCollection OfflineVertexCollection;
    Handle<vector<reco::Vertex>> offlineVertexHandle;
    bool foundOfflineVertexColl = iEvent.getByToken(offlineVertexToken_, offlineVertexHandle);
    if( foundOfflineVertexColl ) OfflineVertexCollection = *offlineVertexHandle;

    for ( auto const& hltVertex : HLTVertexCollection ) {
        //LogWarning("hltvertex") << " ndof: " << hltVertex.ndof() << " z: " << hltVertex.z() << " rho: " << hltVertex.position().Rho();
       
        histos1D_["hlt_ndof"]->Fill( hltVertex.ndof() );
        histos1D_["hlt_z"]->Fill( hltVertex.z() );
        histos1D_["hlt_rho"]->Fill( hltVertex.position().Rho() );
        histos1D_["hlt_ntracks"]->Fill( hltVertex.nTracks() );
        histos1D_["hlt_chi2"]->Fill( hltVertex.chi2() );
        histos1D_["hlt_normalizedchi2"]->Fill( hltVertex.normalizedChi2() );
        histos1D_["hlt_trackssize"]->Fill( hltVertex.tracksSize() );
        
        double minDeltaR = 9999;
        TLorentzVector tmphltVertex;
        tmphltVertex.SetXYZT( hltVertex.x(), hltVertex.y(), hltVertex.z(), hltVertex.t() );
        for ( auto const& offlineVertex : OfflineVertexCollection ) {
            TLorentzVector tmpofflineVertex;
            tmpofflineVertex.SetXYZT( offlineVertex.x(), offlineVertex.y(), offlineVertex.z(), offlineVertex.t() );
            double tmpDeltaR = tmphltVertex.DeltaR( tmpofflineVertex );
            if ( tmpDeltaR < minDeltaR ) {
                minDeltaR= tmpDeltaR;
            }
                ///// HERE YOU NEED TO INCLUDE YOUR PLOTS, I JUST CREATE THE LOOP
        }      
    }

    for ( auto const& offlineVertex : OfflineVertexCollection ) {
        //LogWarning("offlinevertex") << offlineVertex.ndof();

        histos1D_["offline_ndof"]->Fill( offlineVertex.ndof() );
        histos1D_["offline_z"]->Fill( offlineVertex.z() );
        histos1D_["offline_rho"]->Fill( offlineVertex.position().Rho() );
        histos1D_["offline_ntracks"]->Fill( offlineVertex.nTracks() );
        histos1D_["offline_chi2"]->Fill( offlineVertex.chi2() );
        histos1D_["offline_normalizedchi2"]->Fill( offlineVertex.normalizedChi2() );
        histos1D_["offline_trackssize"]->Fill( offlineVertex.tracksSize() );
    }
}


// ------------ method called once each job just before starting event loop  ------------
void vertexAnalyzer::beginJob() {

  histos1D_[ "hlt_ndof" ] = fs_->make< TH1D >( "hlt_ndof", "hlt_ndof", 200, 0.0, 200.0 );
  histos1D_[ "hlt_z" ] = fs_->make< TH1D >( "hlt_z", "hlt_z", 100, -20.0, 20.0 );
  histos1D_[ "hlt_rho" ] = fs_->make< TH1D >( "hlt_rho", "hlt_rho", 100, -0.5, 0.5 );
  histos1D_[ "hlt_ntracks" ] = fs_->make< TH1D >( "hlt_ntracks", "hlt_ntracks", 100, 0.0, 100.0 );
  histos1D_[ "hlt_chi2" ] = fs_->make< TH1D >( "hlt_chi2", "hlt_chi2", 100, 0.0, 10.0 );
  histos1D_[ "hlt_normalizedchi2" ] = fs_->make< TH1D >( "hlt_normalizedchi2", "hlt_normalizedchi2", 100, 0.0, 10.0 );
  histos1D_[ "hlt_trackssize" ] = fs_->make< TH1D >( "hlt_trackssize", "hlt_trackssize", 100, 0.0, 100.0 );

  histos1D_[ "offline_ndof" ] = fs_->make< TH1D >( "offline_ndof", "offline_ndof", 200, 0.0, 200.0 );
  histos1D_[ "offline_z" ] = fs_->make< TH1D >( "offline_z", "offline_z", 100, -20.0, 20.0 );
  histos1D_[ "offline_rho" ] = fs_->make< TH1D >( "offline_rho", "offline_rho", 100, -0.5, 0.5 );
  histos1D_[ "offline_ntracks" ] = fs_->make< TH1D >( "offline_ntracks", "offline_ntracks", 100, 0.0, 100.0 );
  histos1D_[ "offline_chi2" ] = fs_->make< TH1D >( "offline_chi2", "offline_chi2", 100, 0.0, 10.0 );
  histos1D_[ "offline_normalizedchi2" ] = fs_->make< TH1D >( "offline_normalizedchi2", "offline_normalizedchi2", 100, 0.0, 10.0 );
  histos1D_[ "offline_trackssize" ] = fs_->make< TH1D >( "offline_trackssize", "offline_trackssize", 100, 0.0, 100.0 );

  ///// Sumw2 all the histos
  for( auto const& histo : histos1D_ ) histos1D_[ histo.first ]->Sumw2();
  for( auto const& histo : histos2D_ ) histos2D_[ histo.first ]->Sumw2();

}

// ------------ method called once each job just after ending the event loop  ------------
void
vertexAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void vertexAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("hltVertex", InputTag("hltPixelVertices") );
  desc.add<edm::InputTag>("offlineVertex",InputTag("offlinePrimaryVerticesWithBS"));
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(vertexAnalyzer);
