// system include files
#include <memory>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TH1.h>
#include <TTree.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/LorentzVector.h"


#include "DataFormats/Common/interface/Ref.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

using namespace edm;
using namespace std;
using namespace pat;

inline bool jetID( double jetEta, double jetE, double jecFactor, double neutralHadronEnergy, double neutralEmEnergy, double chargedHadronEnergy, double muonEnergy, double chargedEmEnergy, double chargedMultiplicity, double neutralMultiplicity, string jetIDtype ){ 

	double jec = 1. / ( jecFactor );
	double NHF = neutralHadronEnergy * jec;
	double NEMF = neutralEmEnergy * jec;
	double CHF = chargedHadronEnergy * jec;
	double MUF = muonEnergy * jec;
	double CEMF = chargedEmEnergy * jec;
	int NumConst = chargedMultiplicity + neutralMultiplicity ; 
	double CHM = chargedMultiplicity * jec;

	bool id = 0;
	if ( jetIDtype == "looseJetID" ) id = (NHF<0.99 && NEMF<0.99 && NumConst>1) && ((abs(jetEta)<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || abs(jetEta)>2.4) && abs(jetEta)<=2.7;
	else if ( jetIDtype == "tightJetID" ) id = (NHF<0.90 && NEMF<0.90 && NumConst>1) && ((abs(jetEta)<=2.4 && CHF>0 && CHM>0 && CEMF<0.99) || abs(jetEta)>2.4) && abs(jetEta)<=2.7;
	else if ( jetIDtype == "tightLepVetoJetID" ) id = (NHF<0.90 && NEMF<0.90 && NumConst>1 && MUF<0.8) && ((abs(jetEta)<=2.4 && CHF>0 && CHM>0 && CEMF<0.90) || abs(jetEta)>2.4) && abs(jetEta)<=2.7;
	else LogError("jetID") << "jetID function only takes the following jetID names: looseJetID, tightJetID, tightLepVetoJetID.";

	return id;
}


inline bool checkTriggerBitsMiniAOD( TriggerNames triggerNames, Handle<TriggerResults> triggerBits, Handle<PackedTriggerPrescales> triggerPrescales, TString HLTtrigger, bool baselineTrigger  ){

  	bool triggerFired = 0;
	for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
		//LogWarning("all triggers") << triggerNames.triggerName(i) << " " <<  triggerBits->accept(i) << " " << triggerPrescales->getPrescaleForIndex(i);
		if (TString(triggerNames.triggerName(i)).Contains(HLTtrigger) && (triggerBits->accept(i))) {
			triggerFired=1;
			//if ( (triggerPrescales->getPrescaleForIndex(i) == 1) || baselineTrigger ) triggerFired=1;
			//LogWarning("triggerbit") << triggerNames.triggerName(i) << " " <<  triggerBits->accept(i) << " " << triggerPrescales->getPrescaleForIndex(i) << " " <<  triggerPrescales->getPrescaleForIndex(i) << " " << baselineTrigger << " " << triggerFired ;
		}
	}

	return triggerFired;
}	

inline bool checkORListOfTriggerBitsMiniAOD( TriggerNames triggerNames, Handle<TriggerResults> triggerBits, Handle<PackedTriggerPrescales> triggerPrescales, vector<string>  triggerPass, bool baselineTrigger  ){

	vector<bool> triggersFired;
	for (size_t t = 0; t < triggerPass.size(); t++) {
		bool triggerFired = checkTriggerBitsMiniAOD( triggerNames, triggerBits, triggerPrescales, triggerPass[t], baselineTrigger );
		triggersFired.push_back( triggerFired );
		//LogWarning("trigger test") << triggerPass[t] << " " << triggerFired;
		//if ( triggerFired ) LogWarning("trigger fired test") << triggerPass[t] << " " << triggerFired;
	}
	
	bool ORTriggers = any_of(triggersFired.begin(), triggersFired.end(), [](bool v) { return v; }); 
	triggersFired.clear();
	
	return ORTriggers;
}

