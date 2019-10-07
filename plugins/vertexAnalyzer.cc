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

  int count_hlt_events=0; //counter for events with hlt vertices
   
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
    if( foundHLTVertexColl ) 
    {
      HLTVertexCollection = *hltVertexHandle;
      count_hlt_events++; //updating number of events with hlt vertices
    }

    VertexCollection OfflineVertexCollection;
    Handle<vector<reco::Vertex>> offlineVertexHandle;
    bool foundOfflineVertexColl = iEvent.getByToken(offlineVertexToken_, offlineVertexHandle);
    if( foundOfflineVertexColl ) OfflineVertexCollection = *offlineVertexHandle;

    //minimum delta R, ndof, z for entire collection of vertices
    double min_delta_r=10000000;//arbitrary minimum delta R 
    
    double hlt_ndof;
    double hlt_z;
    math::XYZTLorentzVectorD hlt_p4;
    double hlt_eta;
    double hlt_phi;
    double offline_ndof;
    double offline_z;
    math::XYZTLorentzVectorD offline_p4;
    double offline_eta;
    double offline_phi;
    double deta;
    double dphi;
    double delta_r;

    double min_delta_r_event=100000000;//arbitrary minimum delta R per event
    double hlt_ndof_event;
    double hlt_z_event;
    double offline_ndof_event;
    double offline_z_event;

    double sum_min_delta_r_vertices=0;//to compute average min delta r per event
    double count_hlt_vertices=0;

    if (foundHLTVertexColl)
    {
    for (auto const& hltVertex : HLTVertexCollection)
    {
      hlt_ndof=hltVertex.ndof();
      hlt_z=hltVertex.z();
      hlt_p4=hltVertex.p4();
      hlt_eta=hlt_p4.Eta();
      hlt_phi=hlt_p4.Phi();

      histos1D_["hlt_ndof"]->Fill(hlt_ndof);
      histos1D_["hlt_z"]->Fill(hlt_z);
      
      count_hlt_vertices++;
      
      for (auto const& offlineVertex : OfflineVertexCollection )
      {
	offline_ndof=offlineVertex.ndof();
	offline_z=offlineVertex.z();
	offline_p4=offlineVertex.p4();
	offline_eta=offline_p4.Eta();
	offline_phi=offline_p4.Phi();

	histos1D_["offline_ndof"]->Fill(offline_ndof);
	histos1D_["offline_z"]->Fill(offline_z);
	
	deta=hlt_eta-offline_eta; //difference in eta
	dphi=hlt_phi=offline_phi; //difference in phi
	delta_r=sqrt(deta*deta+dphi*dphi);

	if(delta_r<min_delta_r)
	{
	  min_delta_r=delta_r;
	}
      }
      histos1D_["min_delta_r"]->Fill(min_delta_r);
      sum_min_delta_r_vertices+=min_delta_r;

      if(min_delta_r<min_delta_r_event)
      {
	min_delta_r_event=min_delta_r;
	hlt_ndof_event=hlt_ndof;
	hlt_z_event=hlt_z;
	offline_ndof_event=offline_ndof;
	offline_z_event=offline_z;
      }
    }
    histos1D_["offline_ndof_event"]->Fill(offline_ndof_event);
    histos1D_["offline_z_event"]->Fill(offline_z_event);
    histos1D_["hlt_ndof_event"]->Fill(hlt_ndof_event);
    histos1D_["hlt_z_event"]->Fill(hlt_z_event);
    histos1D_["min_delta_r_event"]->Fill(min_delta_r_event);
    histos1D_["average_min_delta_r_event"]->Fill(sum_min_delta_r_vertices/count_hlt_vertices);
    }
}

// ------------ method called once each job just before starting event loop  ------------
void
vertexAnalyzer::beginJob()
{
  histos1D_[ "hlt_ndof" ] = fs_->make< TH1D >( "hlt_ndof", "hlt_ndof", 10, 0.0, 10.0 );
  histos1D_[ "hlt_z" ] = fs_->make< TH1D >( "hlt_z", "hlt_z", 100, -20.0, 20.0 );
  histos1D_[ "hlt_ndof_event" ] = fs_->make< TH1D >( "hlt_ndof_event", "hlt_ndof_event", 10, 0.0, 10.0 );
  histos1D_[ "hlt_z_event" ] = fs_->make< TH1D >( "hlt_z_event", "hlt_z_event", 100, -20.0, 20.0 );

  histos1D_[ "offline_ndof" ] = fs_->make< TH1D >( "offline_ndof", "offline_ndof", 10, 0.0, 10.0 );
  histos1D_[ "offline_z" ] = fs_->make< TH1D >( "offline_z", "offline_z", 100, -20.0, 20.0 );
  histos1D_[ "offline_ndof_event" ] = fs_->make< TH1D >( "offline_ndof_event", "offline_ndof_event", 10, 0.0, 10.0 );
  histos1D_[ "offline_z_event" ] = fs_->make< TH1D >( "offline_z_event", "offline_z_event", 100, -20.0, 20.0 );

  histos1D_[ "min_delta_r" ] = fs_->make< TH1D >( "min_delta_r", "min_delta_r", 50, 0.0, 5.0 );
  histos1D_[ "min_delta_r_event" ] = fs_->make< TH1D >( "min_delta_r_event", "min_delta_r_event", 50, 0.0, 5.0 );
  histos1D_[ "average_min_delta_r_event" ] = fs_->make< TH1D >( "average_min_delta_r_event", "average_min_delta_r_event", 50, 0.0, 5.0 );

  histos1D_["count_hlt_events"]=fs_->make< TH1D >( "count_hlt_events", "count_hlt_events", 500, 0.0, 500.0 );

  ///// Sumw2 all the histos
  for( auto const& histo : histos1D_ ) histos1D_[ histo.first ]->Sumw2();
  for( auto const& histo : histos2D_ ) histos2D_[ histo.first ]->Sumw2();
}

// ------------ method called once each job just after ending the event loop  ------------
void
vertexAnalyzer::endJob()
{
   histos1D_[ "count_hlt_events" ]->Fill(count_hlt_events);
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
