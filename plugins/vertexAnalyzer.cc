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

  int count_hlt=0;
   
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
    if( foundHLTVertexColl ) {
      HLTVertexCollection = *hltVertexHandle;
      count_hlt++;
    }

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

    double mindelevent=100000000;
    double hltndofevent;
    double offlinendofevent;
    double hltzevent;
    double offlinezevent;

    double offline_z;
    double hlt_z;
    double offline_ndof;
    double hlt_ndof;

    for (auto const& hltVertex : HLTVertexCollection ){

      mindel=1000000; //arbitrary min deltaR

      hlt_ndof=hltVertex.ndof();
      hlt_z=hltVertex.z();

      hlt_p4=hltVertex.p4(); 
      hlt_eta=hlt_p4.Eta();
      hlt_phi=hlt_p4.Phi();

      for (auto const& offlineVertex : OfflineVertexCollection ){

	offline_p4=offlineVertex.p4();
	offline_eta=offline_p4.Eta();
	offline_phi=offline_p4.Phi();

	offline_ndof=offlineVertex.ndof();
	offline_z=offlineVertex.z();
    
	deta=hlt_eta-offline_eta;
	dphi=hlt_phi-offline_phi;

    	del=sqrt(deta*deta+dphi*dphi);

    	if (del<mindel){
    	  mindel=del;} //mindel for each hlt vertex
    	
      }
    histos1D_[ "mindeltar" ]->Fill(mindel);

    //per event
    if (mindel<mindelevent){
      mindelevent=mindel;
      hltndofevent=hlt_ndof;
      offlinendofevent=offline_ndof;
      hltzevent=hlt_z;
      offlinezevent=offline_z;
      }
    
    }
    if(mindelevent!=100000000){
      histos1D_[ "mindeltar_event" ]->Fill(mindelevent);
      histos1D_[ "hlt_ndof_event" ]->Fill(hltndofevent);
      histos1D_[ "offline_ndof_event" ]->Fill(offlinendofevent);
      histos1D_[ "hlt_z_event" ]->Fill(hltzevent);
      histos1D_[ "offline_z_event" ]->Fill(offlinezevent);
    }
}


// ------------ method called once each job just before starting event loop  ------------
void
vertexAnalyzer::beginJob()
{
  histos1D_[ "hltvertex_ndof" ] = fs_->make< TH1D >( "hltvertex_ndof", "hltvertex_ndof", 10, 0.0, 10.0 );
  histos1D_[ "hltvertex_z" ] = fs_->make< TH1D >( "hltvertex_z", "hltvertex_z", 100, -20.0, 20.0 );
  histos1D_[ "hlt_ndof_event" ] = fs_->make< TH1D >( "hlt_ndof_event", "hlt_ndof_event", 10, 0.0, 10.0 );
  histos1D_[ "hlt_z_event" ] = fs_->make< TH1D >( "hlt_z_event", "hlt_z_event", 100, -20.0, 20.0 );

  histos1D_[ "offlinevertex_ndof" ] = fs_->make< TH1D >( "offlinevertex_ndof", "offlinevertex_ndof", 10, 0.0, 10.0 );
  histos1D_[ "offlinevertex_z" ] = fs_->make< TH1D >( "offlinevertex_z", "offlinevertex_z", 100, -20.0, 20.0 );
  histos1D_[ "offline_ndof_event" ] = fs_->make< TH1D >( "offline_ndof_event", "offline_ndof_event", 10, 0.0, 10.0 );
  histos1D_[ "offline_z_event" ] = fs_->make< TH1D >( "offline_z_event", "offline_z_event", 100, -20.0, 20.0 );

  histos1D_[ "mindeltar" ] = fs_->make< TH1D >( "mindeltar", "mindeltar", 50, 0.0, 5.0 );
  histos1D_[ "mindeltar_event" ] = fs_->make< TH1D >( "mindeltarevent", "mindeltarevent", 50, 0.0, 3.0 );

  histos1D_["dummy_hlt"]=fs_->make< TH1D >( "dummy_hlt", "dummy_hlt", 500, 0.0, 500.0 );

  ///// Sumw2 all the histos
  for( auto const& histo : histos1D_ ) histos1D_[ histo.first ]->Sumw2();
  for( auto const& histo : histos2D_ ) histos2D_[ histo.first ]->Sumw2();
}

// ------------ method called once each job just after ending the event loop  ------------
void
vertexAnalyzer::endJob()
{
   histos1D_[ "dummy_hlt" ]->Fill(count_hlt);
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
