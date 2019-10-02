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
#include <cmath>
#include<math.h>
#include <TH1.h>
#include <TH2.h>
#include <TLorentzVector.h>


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "TLorentzVector.h"

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
       histos1D_[ "hltvertex_ndof" ]->Fill(hltVertex.ndof());
       histos1D_[ "hltvertex_z" ]->Fill(hltVertex.z());
    }

    for ( auto const& offlineVertex : OfflineVertexCollection ) {
       histos1D_[ "offlinevertex_ndof" ]->Fill(offlineVertex.ndof());
       histos1D_[ "offlinevertex_z" ]->Fill(offlineVertex.z()); 

    }

    //deltaR
    double mindel;
    double del;
    double hlt_eta;
    double hlt_phi;
    double offline_eta;
    double offline_phi;
    double deta;
    double dphi;
    math::XYZTLorentzVectorD hlt_p4;
    math::XYZTLorentzVectorD offline_p4;

    for (auto const& hltVertex : HLTVertexCollection ){

      mindel=1000000; //arbitrary min deltaR

      hlt_p4=hltVertex.p4(); 
      hlt_eta=hlt_p4.Eta();
      hlt_phi=hlt_p4.Phi();

      for (auto const& offlineVertex : OfflineVertexCollection ){

	offline_p4=offlineVertex.p4();
	offline_eta=offline_p4.Eta();
	offline_phi=offline_p4.Phi();
    
	deta=hlt_eta-offline_eta;
	dphi=hlt_phi-offline_phi;

    	del=sqrt(deta*deta+dphi*dphi);

    	if (del<mindel){
    	  mindel=del;
    	}
    }
    histos1D_[ "mindeltar" ]->Fill(mindel);
    }
  

}


// ------------ method called once each job just before starting event loop  ------------
void
vertexAnalyzer::beginJob()
{
  histos1D_[ "hltvertex_ndof" ] = fs_->make< TH1D >( "hltvertex_ndof", "hltvertex_ndof", 20, 0.0, 10.0 );
  histos1D_[ "hltvertex_z" ] = fs_->make< TH1D >( "hltvertex_z", "hltvertex_z", 200, -50.0, 50.0 );

  histos1D_[ "offlinevertex_ndof" ] = fs_->make< TH1D >( "offlinevertex_ndof", "offlinevertex_ndof", 20, 0.0, 10.0 );
  histos1D_[ "offlinevertex_z" ] = fs_->make< TH1D >( "offlinevertex_z", "offlinevertex_z", 200, -50.0, 50.0 );

  histos1D_[ "mindeltar" ] = fs_->make< TH1D >( "mindeltar", "mindeltar", 200, 0.0, 50.0 );

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
