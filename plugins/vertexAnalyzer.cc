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

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
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
        EDGetTokenT<TriggerResults> triggerBits_;

    string baseTrigger_;
    vector<string> triggerPass_;
    bool DEBUG_;

    Service<TFileService> fs_;
    map< string, TH1D* > histos1D_;
    map< string, TH2D* > histos2D_;

  int count_hlt_events=0; //counter for events with hlt vertices
  int trigger_count_hlt_events=0;
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
vertexAnalyzer::vertexAnalyzer(const edm::ParameterSet& iConfig):
  hltVertexToken_(consumes<vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("hltVertex"))),
  offlineVertexToken_(consumes<vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("offlineVertex"))),

  triggerBits_(consumes<TriggerResults>(iConfig.getParameter<InputTag>("bits")))

{
   baseTrigger_ = iConfig.getParameter<string>("baseTrigger");
   triggerPass_ = iConfig.getParameter<vector<string>>("triggerPass");

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

    Handle<TriggerResults> triggerBits;
    iEvent.getByToken(triggerBits_, triggerBits);
    const TriggerNames &names = iEvent.triggerNames(*triggerBits);

    //minimum delta R, ndof, z, rho for entire collection of vertices
    double min_delta_r=10000000;//arbitrary minimum delta R 
    
    double hlt_ndof;
    double hlt_z;
    math::XYZTLorentzVectorD hlt_p4;
    double hlt_eta;
    double hlt_phi;
    double hlt_rho;
    double hlt_chi2;
    double hlt_normalizedchi2;
    int hlt_ntracks;
    size_t hlt_trackssize;
    double count_hlt_pv=0;

    double offline_ndof;
    double offline_z;
    math::XYZTLorentzVectorD offline_p4;
    double offline_eta;
    double offline_phi;
    double offline_rho;
    double offline_chi2;
    double offline_normalizedchi2;
    int offline_ntracks;
    size_t offline_trackssize;
    double count_offline_pv=0;

    double deta;
    double dphi;
    double delta_r;

    double min_delta_r_event=100000000;//arbitrary minimum delta R per event
    double hlt_ndof_event;
    double hlt_z_event;
    double hlt_rho_event;
    double offline_ndof_event;
    double offline_z_event;
    double offline_rho_event;

    double sum_min_delta_r_vertices=0;//to compute average min delta r per event
    double count_hlt_vertices=0;

    if (foundHLTVertexColl)
    {
    for (auto const& hltVertex : HLTVertexCollection)
    {
      hlt_ndof=hltVertex.ndof();
      hlt_z=hltVertex.z();
      hlt_rho=hltVertex.position().Rho();
      hlt_chi2=hltVertex.chi2();
      hlt_normalizedchi2=hltVertex.normalizedChi2();
      hlt_ntracks=hltVertex.nTracks();
      hlt_trackssize=hltVertex.tracksSize();
      hlt_p4=hltVertex.p4();
      hlt_eta=hlt_p4.Eta();
      hlt_phi=hlt_p4.Phi();

      if((hlt_ndof>4) && (fabs(hlt_z)<=24) && (fabs(hlt_rho)<=2.0))
      {
	count_hlt_pv++;
      }

      histos1D_["hlt_ndof"]->Fill(hlt_ndof);
      histos1D_["hlt_z"]->Fill(hlt_z);
      histos1D_["hlt_rho"]->Fill(hlt_rho);
      histos1D_["hlt_ntracks"]->Fill(hlt_ntracks);
      histos1D_["hlt_chi2"]->Fill(hlt_chi2);
      histos1D_["hlt_normalizedchi2"]->Fill(hlt_normalizedchi2);
      histos1D_["hlt_trackssize"]->Fill(hlt_trackssize);
  
      count_hlt_vertices++;
      
      for (auto const& offlineVertex : OfflineVertexCollection )
      {
	offline_ndof=offlineVertex.ndof();
	offline_z=offlineVertex.z();
	offline_rho=offlineVertex.position().Rho();
	offline_chi2=offlineVertex.chi2();
	offline_normalizedchi2=offlineVertex.normalizedChi2();
	offline_ntracks=offlineVertex.nTracks();
	offline_trackssize=offlineVertex.tracksSize();
	offline_p4=offlineVertex.p4();
	offline_eta=offline_p4.Eta();
	offline_phi=offline_p4.Phi();

	if((offline_ndof>4) && (fabs(offline_z)<=24) && (fabs(offline_rho)<=2.0))
	{
	  count_offline_pv++;
	}

	histos1D_["offline_ndof"]->Fill(offline_ndof);
	histos1D_["offline_z"]->Fill(offline_z);
	histos1D_["offline_rho"]->Fill(offline_rho);
	histos1D_["offline_ntracks"]->Fill(offline_ntracks);
	histos1D_["offline_chi2"]->Fill(offline_chi2);
	histos1D_["offline_normalizedchi2"]->Fill(offline_normalizedchi2);
	histos1D_["offline_trackssize"]->Fill(offline_trackssize);
	
	deta=hlt_eta-offline_eta; //difference in eta
	dphi=hlt_phi=offline_phi; //difference in phi
	delta_r=sqrt(deta*deta+dphi*dphi);

	if(delta_r<min_delta_r)
	{
	  min_delta_r=delta_r;
	}
      }//end of offlineVertexCollection loop
      histos1D_["min_delta_r"]->Fill(min_delta_r);
      sum_min_delta_r_vertices+=min_delta_r;

      if(min_delta_r<min_delta_r_event)
      {
	min_delta_r_event=min_delta_r;
	hlt_ndof_event=hlt_ndof;
	hlt_z_event=hlt_z;
	hlt_rho_event=hlt_rho;
	offline_ndof_event=offline_ndof;
	offline_z_event=offline_z;
	offline_rho_event=offline_rho;
      }
    }// end of hltVertexCollection loop
    histos1D_["offline_ndof_event"]->Fill(offline_ndof_event);
    histos1D_["offline_z_event"]->Fill(offline_z_event);
    histos1D_["offline_rho_event"]->Fill(offline_rho_event);
    histos1D_["hlt_ndof_event"]->Fill(hlt_ndof_event);
    histos1D_["hlt_z_event"]->Fill(hlt_z_event);
    histos1D_["hlt_rho_event"]->Fill(hlt_rho_event);
    histos1D_["min_delta_r_event"]->Fill(min_delta_r_event);
    histos1D_["average_min_delta_r_event"]->Fill(sum_min_delta_r_vertices/count_hlt_vertices);
    histos1D_["count_hlt_pv_event"]->Fill(count_hlt_pv);
    histos1D_["count_offline_pv_event"]->Fill(count_offline_pv);
    }//end of if (foundHLTVertexColl) loop

    // Checking if the triggers were fired
    bool baseTrigger = 0;
    bool ORTriggers = 0;
    vector<bool> triggersFired;
    for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) 
    {
     if (DEBUG_) LogWarning("all triggers") << names.triggerName(i) << " " <<  triggerBits->accept(i);
     if (TString(names.triggerName(i)).Contains(baseTrigger_) && (triggerBits->accept(i)))  baseTrigger = true;
     for (size_t t = 0; t < triggerPass_.size(); t++) 
     {
      if (TString(names.triggerName(i)).Contains(triggerPass_[t]) && (triggerBits->accept(i))) triggersFired.push_back( true );
      else triggersFired.push_back( false );
     }
    }
    ORTriggers = any_of(triggersFired.begin(), triggersFired.end(), [](bool v) { return v; }); 
    triggersFired.clear();
    if (DEBUG_) LogWarning("trigger fired") << "Based " << baseTrigger << " OR " << ORTriggers;
    if ( TString(baseTrigger_).Contains("empty") ) baseTrigger = true;

    if ( baseTrigger || ORTriggers )// checking if both triggers passed
    {
      
    double min_delta_r=10000000;//arbitrary minimum delta R 
    
    double hlt_ndof;
    double hlt_z;
    math::XYZTLorentzVectorD hlt_p4;
    double hlt_eta;
    double hlt_phi;
    double hlt_rho;
    double count_hlt_pv=0;

    double offline_ndof;
    double offline_z;
    math::XYZTLorentzVectorD offline_p4;
    double offline_eta;
    double offline_phi;
    double offline_rho;
    double count_offline_pv=0;

    double deta;
    double dphi;
    double delta_r;

    double min_delta_r_event=100000000;//arbitrary minimum delta R per event
    double hlt_ndof_event;
    double hlt_z_event;
    double hlt_rho_event;
    double offline_ndof_event;
    double offline_z_event;
    double offline_rho_event;

    double sum_min_delta_r_vertices=0;//to compute average min delta r per event
    double count_hlt_vertices=0;

    if (foundHLTVertexColl)
    {
    trigger_count_hlt_events++;
    for (auto const& hltVertex : HLTVertexCollection)
    {
      hlt_ndof=hltVertex.ndof();
      hlt_z=hltVertex.z();
      hlt_rho=hltVertex.position().Rho();
      hlt_p4=hltVertex.p4();
      hlt_eta=hlt_p4.Eta();
      hlt_phi=hlt_p4.Phi();

      if((hlt_ndof>4) && (fabs(hlt_z)<=24) && (fabs(hlt_rho)<=2.0))
      {
	count_hlt_pv++;
      }

      histos1D_["trigger_hlt_ndof"]->Fill(hlt_ndof);
      histos1D_["trigger_hlt_z"]->Fill(hlt_z);
      histos1D_["trigger_hlt_rho"]->Fill(hlt_rho);
      
      count_hlt_vertices++;
      
      for (auto const& offlineVertex : OfflineVertexCollection )
      {
	offline_ndof=offlineVertex.ndof();
	offline_z=offlineVertex.z();
	offline_rho=offlineVertex.position().Rho();
	offline_p4=offlineVertex.p4();
	offline_eta=offline_p4.Eta();
	offline_phi=offline_p4.Phi();

	if((offline_ndof>4) && (fabs(offline_z)<=24) && (fabs(offline_rho)<=2.0))
	{
	  count_offline_pv++;
	}

	histos1D_["trigger_offline_ndof"]->Fill(offline_ndof);
	histos1D_["trigger_offline_z"]->Fill(offline_z);
	histos1D_["trigger_offline_rho"]->Fill(offline_rho);
	
	deta=hlt_eta-offline_eta; //difference in eta
	dphi=hlt_phi=offline_phi; //difference in phi
	delta_r=sqrt(deta*deta+dphi*dphi);

	if(delta_r<min_delta_r)
	{
	  min_delta_r=delta_r;
	}
      }//end of hltVertexCollection loop
      histos1D_["trigger_min_delta_r"]->Fill(min_delta_r);
      sum_min_delta_r_vertices+=min_delta_r;

      if(min_delta_r<min_delta_r_event)
      {
	min_delta_r_event=min_delta_r;
	hlt_ndof_event=hlt_ndof;
	hlt_z_event=hlt_z;
	hlt_rho_event=hlt_rho;
	offline_ndof_event=offline_ndof;
	offline_z_event=offline_z;
	offline_rho_event=offline_rho;
      }
    }// end of offlineVertexCollection loop
    histos1D_["trigger_offline_ndof_event"]->Fill(offline_ndof_event);
    histos1D_["trigger_offline_z_event"]->Fill(offline_z_event);
    histos1D_["trigger_offline_rho_event"]->Fill(offline_rho_event);
    histos1D_["trigger_hlt_ndof_event"]->Fill(hlt_ndof_event);
    histos1D_["trigger_hlt_z_event"]->Fill(hlt_z_event);
    histos1D_["trigger_hlt_rho_event"]->Fill(hlt_rho_event);
    histos1D_["trigger_min_delta_r_event"]->Fill(min_delta_r_event);
    histos1D_["trigger_average_min_delta_r_event"]->Fill(sum_min_delta_r_vertices/count_hlt_vertices);
    histos1D_["trigger_count_hlt_pv_event"]->Fill(count_hlt_pv);
    histos1D_["count_offline_pv_event"]->Fill(count_offline_pv);
    }//end of if (foundHLTVertexColl) loop
    }//end of if both triggers fired loop

}

// ------------ method called once each job just before starting event loop  ------------
void
vertexAnalyzer::beginJob()
{
  histos1D_[ "hlt_ndof" ] = fs_->make< TH1D >( "hlt_ndof", "hlt_ndof", 10, 0.0, 10.0 );
  histos1D_[ "hlt_z" ] = fs_->make< TH1D >( "hlt_z", "hlt_z", 100, -20.0, 20.0 );
  histos1D_[ "hlt_rho" ] = fs_->make< TH1D >( "hlt_rho", "hlt_rho", 100, -0.5, 0.5 );
  histos1D_[ "hlt_ntracks" ] = fs_->make< TH1D >( "hlt_ntracks", "hlt_ntracks", 200, 0.0, 100.0 );
  histos1D_[ "hlt_chi2" ] = fs_->make< TH1D >( "hlt_chi2", "hlt_chi2", 100, 0.0, 10.0 );
  histos1D_[ "hlt_normalizedchi2" ] = fs_->make< TH1D >( "hlt_normalizedchi2", "hlt_normalizedchi2", 100, 0.0, 10.0 );
  histos1D_[ "hlt_trackssize" ] = fs_->make< TH1D >( "hlt_trackssize", "hlt_trackssize", 200, 0.0, 100.0 );
  histos1D_[ "hlt_ndof_event" ] = fs_->make< TH1D >( "hlt_ndof_event", "hlt_ndof_event", 10, 0.0, 10.0 );
  histos1D_[ "hlt_z_event" ] = fs_->make< TH1D >( "hlt_z_event", "hlt_z_event", 100, -20.0, 20.0 );
  histos1D_[ "hlt_rho_event" ] = fs_->make< TH1D >( "hlt_rho_event", "hlt_rho_event", 100, -0.5, 0.5 );

  histos1D_[ "offline_ndof" ] = fs_->make< TH1D >( "offline_ndof", "offline_ndof", 10, 0.0, 10.0 );
  histos1D_[ "offline_z" ] = fs_->make< TH1D >( "offline_z", "offline_z", 100, -20.0, 20.0 );
  histos1D_[ "offline_rho" ] = fs_->make< TH1D >( "offline_rho", "offline_rho", 100, -0.5, 0.5 );
  histos1D_[ "offline_ntracks" ] = fs_->make< TH1D >( "offline_ntracks", "offline_ntracks", 200, 0.0, 100.0 );
  histos1D_[ "offline_chi2" ] = fs_->make< TH1D >( "offline_chi2", "offline_chi2", 100, 0.0, 10.0 );
  histos1D_[ "offline_normalizedchi2" ] = fs_->make< TH1D >( "offline_normalizedchi2", "offline_normalizedchi2", 100, 0.0, 10.0 );
  histos1D_[ "offline_trackssize" ] = fs_->make< TH1D >( "offline_trackssize", "offline_trackssize", 200, 0.0, 100.0 );
  histos1D_[ "offline_ndof_event" ] = fs_->make< TH1D >( "offline_ndof_event", "offline_ndof_event", 10, 0.0, 10.0 );
  histos1D_[ "offline_z_event" ] = fs_->make< TH1D >( "offline_z_event", "offline_z_event", 100, -20.0, 20.0 );
  histos1D_[ "offline_rho_event" ] = fs_->make< TH1D >( "offline_rho_event", "offline_rho_event", 100, -0.5, 0.5 );

  histos1D_[ "min_delta_r" ] = fs_->make< TH1D >( "min_delta_r", "min_delta_r", 50, 0.0, 5.0 );
  histos1D_[ "min_delta_r_event" ] = fs_->make< TH1D >( "min_delta_r_event", "min_delta_r_event", 50, 0.0, 5.0 );
  histos1D_[ "average_min_delta_r_event" ] = fs_->make< TH1D >( "average_min_delta_r_event", "average_min_delta_r_event", 50, 0.0, 5.0 );

  histos1D_["count_hlt_events"]=fs_->make< TH1D >( "count_hlt_events", "count_hlt_events", 500, 0.0, 500.0 );
  histos1D_["count_hlt_pv_event"]=fs_->make< TH1D >( "count_hlt_pv_event", "count_hlt_pv_event", 500, 0.0, 10000.0 );
  histos1D_["count_offline_pv_event"]=fs_->make< TH1D >( "count_offline_pv_event", "count_offline_pv_event", 500, 0.0, 10000.0 );
  
  histos1D_[ "trigger_hlt_ndof" ] = fs_->make< TH1D >( "trigger_hlt_ndof", "trigger_hlt_ndof", 10, 0.0, 10.0 );
  histos1D_[ "trigger_hlt_z" ] = fs_->make< TH1D >( "trigger_hlt_z", "trigger_hlt_z", 100, -20.0, 20.0 );
  histos1D_[ "trigger_hlt_rho" ] = fs_->make< TH1D >( "trigger_hlt_rho", "trigger_hlt_rho", 100, -0.5, 0.5 );
  histos1D_[ "trigger_hlt_ndof_event" ] = fs_->make< TH1D >( "trigger_hlt_ndof_event", "trigger_hlt_ndof_event", 10, 0.0, 10.0 );
  histos1D_[ "trigger_hlt_z_event" ] = fs_->make< TH1D >( "trigger_hlt_z_event", "trigger_hlt_z_event", 100, -20.0, 20.0 );
  histos1D_[ "trigger_hlt_rho_event" ] = fs_->make< TH1D >( "trigger_hlt_rho_event", "trigger_hlt_rho_event", 100, -0.5, 0.5 );

  histos1D_[ "trigger_offline_ndof" ] = fs_->make< TH1D >( "trigger_offline_ndof", "trigger_offline_ndof", 10, 0.0, 10.0 );
  histos1D_[ "trigger_offline_z" ] = fs_->make< TH1D >( "trigger_offline_z", "trigger_offline_z", 100, -20.0, 20.0 );
  histos1D_[ "trigger_offline_rho" ] = fs_->make< TH1D >( "trigger_offline_rho", "trigger_offline_rho", 100, -0.5, 0.5 );
  histos1D_[ "trigger_offline_ndof_event" ] = fs_->make< TH1D >( "trigger_offline_ndof_event", "trigger_offline_ndof_event", 10, 0.0, 10.0 );
  histos1D_[ "trigger_offline_z_event" ] = fs_->make< TH1D >( "trigger_offline_z_event", "trigger_offline_z_event", 100, -20.0, 20.0 );
  histos1D_[ "trigger_offline_rho_event" ] = fs_->make< TH1D >( "trigger_offline_rho_event", "trigger_offline_rho_event", 100, -0.5, 0.5 );

  histos1D_[ "trigger_min_delta_r" ] = fs_->make< TH1D >( "trigger_min_delta_r", "trigger_min_delta_r", 50, 0.0, 5.0 );
  histos1D_[ "trigger_min_delta_r_event" ] = fs_->make< TH1D >( "trigger_min_delta_r_event", "trigger_min_delta_r_event", 50, 0.0, 5.0 );
  histos1D_[ "trigger_average_min_delta_r_event" ] = fs_->make< TH1D >( "trigger_average_min_delta_r_event", "trigger_average_min_delta_r_event", 50, 0.0, 5.0 );

  histos1D_["trigger_count_hlt_events"]=fs_->make< TH1D >( "trigger_count_hlt_events", "trigger_count_hlt_events", 500, 0.0, 500.0 );
  histos1D_["trigger_count_hlt_pv_event"]=fs_->make< TH1D >( "trigger_count_hlt_pv_event", "trigger_count_hlt_pv_event", 500, 0.0, 10000.0 );
  histos1D_["trigger_count_offline_pv_event"]=fs_->make< TH1D >( "trigger_count_offline_pv_event", "trigger_count_offline_pv_event", 500, 0.0, 10000.0 );

  ///// Sumw2 all the histos
  for( auto const& histo : histos1D_ ) histos1D_[ histo.first ]->Sumw2();
  for( auto const& histo : histos2D_ ) histos2D_[ histo.first ]->Sumw2();
}

// ------------ method called once each job just after ending the event loop  ------------
void
vertexAnalyzer::endJob()
{
   histos1D_[ "count_hlt_events" ]->Fill(count_hlt_events);
   histos1D_[ "trigger_count_hlt_events" ]->Fill(trigger_count_hlt_events);
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void vertexAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("hltVertex", InputTag("hltPixelVertices") );
  desc.add<edm::InputTag>("offlineVertex",InputTag("offlinePrimaryVerticesWithBS"));
  desc.add<InputTag>("bits", 	InputTag("TriggerResults", "", "HLT2"));
  desc.add<string>("baseTrigger", 	"HLT_PFHT800");
  vector<string> HLTPass;
  HLTPass.push_back("HLT_AK8PFHT650_TrimR0p1PT0p03Mass50");
  desc.add<vector<string>>("triggerPass",	HLTPass);
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(vertexAnalyzer);
