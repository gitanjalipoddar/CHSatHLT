// -*- C++ -*-
//
// Package:    PUHLT/simpleAnalyzer
// Class:      simpleAnalyzer
//
/**\class simpleAnalyzer simpleAnalyzer.cc PUHLT/simpleAnalyzer/plugins/simpleAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Alejandro Gomez Espinosa
//         Created:  Wed, 15 May 2019 12:09:45 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
 #include "FWCore/Utilities/interface/InputTag.h"
 #include "DataFormats/TrackReco/interface/Track.h"
 #include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


using namespace reco;

class simpleAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit simpleAnalyzer(const edm::ParameterSet&);
      ~simpleAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      //edm::EDGetTokenT<TrackCollection> tracksToken_;  //used to select what tracks to read from configuration file
      edm::EDGetTokenT <reco::PFJetCollection> ak4jetsToken_;  //used to select what tracks to read from configuration file
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
simpleAnalyzer::simpleAnalyzer(const edm::ParameterSet& iConfig)
 :
  //tracksToken_(consumes<TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tracks")))
  ak4jetsToken_(consumes<PFJetCollection>(iConfig.getParameter<edm::InputTag>("ak4jets")))

{
   //now do what ever initialization is needed

}


simpleAnalyzer::~simpleAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void simpleAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

    using namespace edm;

    //open the trigger summary
    /*edm::InputTag triggerSummaryLabel_ = edm::InputTag("hltTriggerSummaryAOD", "", "TEST");
    edm::Handle<trigger::TriggerEvent> triggerSummary;
    iEvent.getByLabel(triggerSummaryLabel_, triggerSummary)

    //trigger object we want to match
    edm::InputTag filterTag = edm::InputTag("hltL1Mu12EG7L3MuFiltered17", "", "HLT");
    //find the index corresponding to the event 
    size_t filterIndex = (*triggerSummary).filterIndex(filterTag);
    trigger::TriggerObjectCollection allTriggerObjects = triggerSummary->getObjects();
    trigger::TriggerObjectCollection selectedObjects;
    if (filterIndex < (*triggerSummary).sizeFilters()) { 
        //check if the trigger object is present!
        const trigger::Keys &keys  = (*triggerSummary).filterKeys(filterIndex);
        for (size_t j = 0; j < keys.size(); j++) {            
            trigger::TriggerObject foundObject = (allTriggerObjects)[keys[j]];            
            selectedObjects.push_back(foundObject);
        }
    }*/

    /*Handle<PFJetCollection> jets;
     * iEvent.getByToken(pfJetsToken,pfjetColl_);
  if(!pfjetColl_.isValid()) return;
  pfjet = *pfjetColl_;
    iEvent.getByToken(ak4jetsToken_, jets);

    for (const reco::PFJet &j :  *jets) {
        //if (j.pt() < 40 || fabs(j.eta()) > 2.4) continue;
        LogWarning("test") << j.pt();
    }*/

}


// ------------ method called once each job just before starting event loop  ------------
void
simpleAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
simpleAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
simpleAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(simpleAnalyzer);
