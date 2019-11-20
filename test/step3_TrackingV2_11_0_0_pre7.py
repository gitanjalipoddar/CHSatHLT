# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions auto:phase2_realistic -n 10 --era Phase2C8_timing_layer_bar --no_output --runUnscheduled -s RAW2DIGI,L1Reco,RECO,RECOSIM --geometry Extended2023D41 --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Phase2C8_timing_layer_bar_cff import Phase2C8_timing_layer_bar

process = cms.Process('RECO2',Phase2C8_timing_layer_bar)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2026D41Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.RecoSim_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(#"root://xrootd-cms.infn.it//store/mc/PhaseIITDRSpring19DR/QCD_Pt-15to3000_EMEnriched_TuneCP5_13TeV_pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v1/50000/BBB89FE4-5C9D-7842-BDE3-C89FF77630B6.root",
#"file:/eos/cms/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/A7DE6079-B3AE-4743-A5F3-2050EDEB8383.root"
     "file:/eos/cms/store/mc/PhaseIITDRSpring19DR/QCD_Pt_50to80_TuneCP5_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v2/260000/0053B97F-B96D-0244-9074-4878AA9B8BC0.root"

),
#    fileNames = cms.untracked.vstring('file:/eos/cms/store/mc/PhaseIITDRSpring19DR/QCD_Pt-15to3000_TuneCP5_Flat_14TeV-pythia8/GEN-SIM-DIGI-RAW/NoPU_castor_106X_upgrade2023_realistic_v3-v2/270000/07A5023E-D011-E448-87A6-FD9586277D47.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step3 nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

#modifications:
process.calolocalreco = cms.Sequence(process.ecalLocalRecoSequence+process.hcalLocalRecoSequence+process.hbhereco+process.hgcalLocalRecoSequence)

process.particleFlowTmpBarrel.useEGammaFilters = cms.bool(False)
process.particleFlowTmpBarrel.useEGammaElectrons = cms.bool(False)
process.particleFlowTmpBarrel.useEGammaSupercluster = cms.bool(False)
process.particleFlowTmpBarrel.usePFConversions = cms.bool(False)
process.particleFlowTmpBarrel.usePFDecays = cms.bool(False)
process.particleFlowTmpBarrel.usePFElectrons = cms.bool(False)
process.particleFlowTmpBarrel.usePFNuclearInteractions = cms.bool(False)
process.particleFlowTmpBarrel.usePFPhotons = cms.bool(False)
process.particleFlowTmpBarrel.usePFSCEleCalib = cms.bool(False)
process.particleFlowTmpBarrel.usePhotonReg = cms.bool(False)
process.particleFlowTmpBarrel.useProtectionsForJetMET = cms.bool(False)

process.pfTrack.GsfTracksInEvents = cms.bool(False)


#redefining the PFBlockProducer removing displaced tracks
process.particleFlowBlock = cms.EDProducer("PFBlockProducer",
    debug = cms.untracked.bool(False),
    elementImporters = cms.VPSet(
#        cms.PSet(
#            gsfsAreSecondary = cms.bool(False),
#            importerName = cms.string('GSFTrackImporter'),
#            source = cms.InputTag("pfTrackElec"),
#            superClustersArePF = cms.bool(True)
#        ), 
#        cms.PSet(
#            importerName = cms.string('ConvBremTrackImporter'),
#            source = cms.InputTag("pfTrackElec")
#        ), 
        cms.PSet(
            importerName = cms.string('SuperClusterImporter'),
            maximumHoverE = cms.double(0.5),
            minPTforBypass = cms.double(100.0),
            minSuperClusterPt = cms.double(10.0),
            source_eb = cms.InputTag("particleFlowSuperClusterECAL","particleFlowSuperClusterECALBarrel"),
            source_ee = cms.InputTag("particleFlowSuperClusterECAL","particleFlowSuperClusterECALEndcapWithPreshower"),
            source_towers = cms.InputTag("towerMaker"),
            superClustersArePF = cms.bool(True)
        ), 
#        cms.PSet(	
#            importerName = cms.string('ConversionTrackImporter'),
#            source = cms.InputTag("pfConversions")
#        ), 
#        cms.PSet(
#            importerName = cms.string('NuclearInteractionTrackImporter'),
#            source = cms.InputTag("pfDisplacedTrackerVertex")
#        ), 
        cms.PSet(
            DPtOverPtCuts_byTrackAlgo = cms.vdouble(
                10.0, 10.0, 10.0, 10.0, 10.0, 
                5.0
            ),
            NHitCuts_byTrackAlgo = cms.vuint32(
                3, 3, 3, 3, 3, 
                3
            ),
            cleanBadConvertedBrems = cms.bool(True),
            importerName = cms.string('GeneralTracksImporterWithVeto'),
            maxDPtOPt = cms.double(1.0),
            muonSrc = cms.InputTag("muons1stStep"),
            source = cms.InputTag("pfTrack"),
            useIterativeTracking = cms.bool(True),
            veto = cms.InputTag("hgcalTrackCollection","TracksInHGCal")
        ), 
        cms.PSet(
            BCtoPFCMap = cms.InputTag("particleFlowSuperClusterECAL","PFClusterAssociationEBEE"),
            importerName = cms.string('ECALClusterImporter'),
            source = cms.InputTag("particleFlowClusterECAL")
        ), 
        cms.PSet(
            importerName = cms.string('GenericClusterImporter'),
            source = cms.InputTag("particleFlowClusterHCAL")
        ), 
        cms.PSet(
            importerName = cms.string('GenericClusterImporter'),
            source = cms.InputTag("particleFlowBadHcalPseudoCluster")
        ), 
        cms.PSet(
            importerName = cms.string('GenericClusterImporter'),
            source = cms.InputTag("particleFlowClusterHO")
        ), 
        cms.PSet(
            importerName = cms.string('GenericClusterImporter'),
            source = cms.InputTag("particleFlowClusterHF")
        ), 
        cms.PSet(
            importerName = cms.string('GenericClusterImporter'),
            source = cms.InputTag("particleFlowClusterPS")
        ), 
        cms.PSet(
            importerName = cms.string('TrackTimingImporter'),
            timeErrorMap = cms.InputTag("tofPID","sigmat0"),
            timeErrorMapGsf = cms.InputTag("tofPID","sigmat0"),
            timeValueMap = cms.InputTag("tofPID","t0"),
            timeValueMapGsf = cms.InputTag("tofPID","t0")
        )
    ),
    linkDefinitions = cms.VPSet(
        cms.PSet(
            linkType = cms.string('PS1:ECAL'),
            linkerName = cms.string('PreshowerAndECALLinker'),
            useKDTree = cms.bool(True)
        ), 
        cms.PSet(
            linkType = cms.string('PS2:ECAL'),
            linkerName = cms.string('PreshowerAndECALLinker'),
            useKDTree = cms.bool(True)
        ), 
        cms.PSet(
            linkType = cms.string('TRACK:ECAL'),
            linkerName = cms.string('TrackAndECALLinker'),
            useKDTree = cms.bool(True)
        ), 
        cms.PSet(
            linkType = cms.string('TRACK:HCAL'),
            linkerName = cms.string('TrackAndHCALLinker'),
            useKDTree = cms.bool(True)
        ), 
        cms.PSet(
            linkType = cms.string('TRACK:HO'),
            linkerName = cms.string('TrackAndHOLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('ECAL:HCAL'),
            linkerName = cms.string('ECALAndHCALLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('HCAL:HO'),
            linkerName = cms.string('HCALAndHOLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('HFEM:HFHAD'),
            linkerName = cms.string('HFEMAndHFHADLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('TRACK:TRACK'),
            linkerName = cms.string('TrackAndTrackLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('ECAL:ECAL'),
            linkerName = cms.string('ECALAndECALLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('GSF:ECAL'),
            linkerName = cms.string('GSFAndECALLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('TRACK:GSF'),
            linkerName = cms.string('TrackAndGSFLinker'),
            useConvertedBrems = cms.bool(True),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('GSF:BREM'),
            linkerName = cms.string('GSFAndBREMLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('GSF:GSF'),
            linkerName = cms.string('GSFAndGSFLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('ECAL:BREM'),
            linkerName = cms.string('ECALAndBREMLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('GSF:HCAL'),
            linkerName = cms.string('GSFAndHCALLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            linkType = cms.string('HCAL:BREM'),
            linkerName = cms.string('HCALAndBREMLinker'),
            useKDTree = cms.bool(False)
        ), 
        cms.PSet(
            SuperClusterMatchByRef = cms.bool(True),
            linkType = cms.string('SC:ECAL'),
            linkerName = cms.string('SCAndECALLinker'),
            useKDTree = cms.bool(False)
        )
    ),
    verbose = cms.untracked.bool(False)
)
#redefining jetGlobalReco
process.jetGlobalReco = cms.Sequence(process.recoJets)

process.ak4PFJets.src = cms.InputTag("particleFlowTmp")


process.itLocalReco = cms.Sequence(
process.siPhase2Clusters + 
process.siPixelClusters + 
process.siPixelClusterShapeCache + 
process.siPixelClustersPreSplitting + 
process.siPixelRecHits + 
process.siPixelRecHitsPreSplitting
)
process.otLocalReco = cms.Sequence(
process.MeasurementTrackerEvent #+
 #clusterSummaryProducer     # not sure what it is :(
)

process.initialStepPVSequence = cms.Sequence(
process.firstStepPrimaryVerticesUnsorted +
process.initialStepTrackRefsForJets +
process.caloTowerForTrk +
process.ak4CaloJetsForTrk +
process.firstStepPrimaryVertices
)
process.initialStepSequence = cms.Sequence(
process.initialStepSeedLayers + 
process.initialStepTrackingRegions + 
process.initialStepHitDoublets + 
    process.initialStepHitQuadruplets + 
    process.initialStepSeeds + 
    process.initialStepTrackCandidates + 
    process.initialStepTracks +
    process.initialStepPVSequence +
    process.initialStepSelector
)

process.highPtTripletStepSequence = cms.Sequence(
    process.highPtTripletStepClusters +
    process.highPtTripletStepSeedLayers +
    process.highPtTripletStepTrackingRegions +
    process.highPtTripletStepHitDoublets +
    process.highPtTripletStepHitTriplets +
    process.highPtTripletStepSeedLayers +
    process.highPtTripletStepSeeds +
    process.highPtTripletStepTrackCandidates +
    process.highPtTripletStepTracks +
    process.highPtTripletStepSelector +
    process.initialStepSeedClusterMask + # needed by electron, but also by process.highPtTripletStepSeedClusterMask
    process.highPtTripletStepSeedClusterMask
)

process.lowPtQuadStepSequence = cms.Sequence(
    process.lowPtQuadStepClusters + 
    process.lowPtQuadStepSeedLayers + 
    process.lowPtQuadStepTrackingRegions + 
    process.lowPtQuadStepHitDoublets + 
    process.lowPtQuadStepHitQuadruplets + 
    process.lowPtQuadStepSeeds + 
    process.lowPtQuadStepTrackCandidates + 
    process.lowPtQuadStepTracks + 
    process.lowPtQuadStepSelector
)

process.lowPtTripletStepSequence = cms.Sequence(
    process.lowPtTripletStepClusters + 
    process.lowPtTripletStepSeedLayers + 
    process.lowPtTripletStepTrackingRegions +
    process.lowPtTripletStepHitDoublets +
    process.lowPtTripletStepHitTriplets +
    process.lowPtTripletStepSeeds + 
    process.lowPtTripletStepTrackCandidates + 
    process.lowPtTripletStepTracks +
    process.lowPtTripletStepSelector
)
process.detachedQuadStepSequence = cms.Sequence(
    process.detachedQuadStepClusters +
    process.detachedQuadStepSeedLayers +
    process.detachedQuadStepTrackingRegions +
    process.detachedQuadStepHitDoublets +
    process.detachedQuadStepHitQuadruplets +
    process.detachedQuadStepSeeds +
    process.detachedQuadStepTrackCandidates +
    process.detachedQuadStepTracks +
    process.detachedQuadStepSelector +
    process.detachedQuadStep
)
process.pixelPairStepSequence = cms.Sequence(
    process.pixelPairStepClusters + 
    process.pixelPairStepSeedLayers + 
    process.pixelPairStepTrackingRegions + 
    process.pixelPairStepHitDoublets + 
    process.pixelPairStepSeeds + 
    process.pixelPairStepTrackCandidates + 
    process.pixelPairStepTracks + 
    process.pixelPairStepSelector 
#    process.pixelPairStepSeedClusterMask # used only by electron !
)
process.muonSeededTracksOutInSequence = cms.Sequence(
    process.muonSeededSeedsOutIn + 
    process.muonSeededTrackCandidatesOutIn + 
    process.muonSeededTracksOutIn + 
    process.muonSeededTracksOutInSelector
)
process.muonSeededTracksInOutSequence = cms.Sequence(
    process.muonSeededSeedsInOut + 
    process.muonSeededTrackCandidatesInOut + 
    process.muonSeededTracksInOut + 
    process.muonSeededTracksInOutSelector
)

process.muonSeededStepSequence = cms.Sequence(
    process.earlyMuons +
    process.muonSeededTracksOutInSequence +
    process.muonSeededTracksInOutSequence
)
process.generalTracks = cms.EDProducer("TrackListMerger",  ### v1
    Epsilon = cms.double(-0.001),
    FoundHitBonus = cms.double(5.0),
    LostHitPenalty = cms.double(5.0),
    MaxNormalizedChisq = cms.double(1000.0),
    MinFound = cms.int32(3),
    MinPT = cms.double(0.05),
    ShareFrac = cms.double(0.19),
    TrackProducers = cms.VInputTag(
        "initialStepTracks", "highPtTripletStepTracks" ### v2
    ),
    allowFirstHitShare = cms.bool(True),
    copyExtras = cms.untracked.bool(True),
    copyMVA = cms.bool(True),
    hasSelector = cms.vint32(
        1, 1#, 1#, 1, 1,  ### v2
        #1
    ),
    indivShareFrac = cms.vdouble(
        1.0, 0.16#, 0.095, 0.09, 0.09, ### v2
        #0.09
    ),
    makeReKeyedSeeds = cms.untracked.bool(False),
    newQuality = cms.string('confirmed'),
    selectedTrackQuals = cms.VInputTag(
        cms.InputTag("initialStepSelector","initialStep"), cms.InputTag("highPtTripletStepSelector","highPtTripletStep")### v2
    ),
    setsToMerge = cms.VPSet(cms.PSet(
        pQual = cms.bool(True),
        tLists = cms.vint32(
            0, 1#, 2#, 3, 4, ### v2
            #5
        )
    )),
    trackAlgoPriorityOrder = cms.string('trackAlgoPriorityOrder'),
    writeOnlyTrkQuals = cms.bool(False)
)

process.globalreco_tracking = cms.Sequence(
	process.offlineBeamSpot+
    process.itLocalReco +
    process.otLocalReco +
    process.calolocalreco +
    process.standalonemuontracking+#we need to add it back for early muons
    process.trackerClusterCheck + 
    process.initialStepSequence +
    process.highPtTripletStepSequence +
    #process.lowPtQuadStepSequence +
    #process.lowPtTripletStepSequence +
    #process.detachedQuadStepSequence +
    #process.pixelPairStepSequence +
    #process.earlyGeneralTracks +
    #process.muonSeededStepSequence +
    #process.preDuplicateMergingGeneralTracks + 
    #process.duplicateTrackCandidates +
    #process.mergedDuplicateTracks + 
    #process.duplicateTrackClassifier + 
    process.generalTracks +
    process.vertexreco
)

#removing un-necessary steps for hgcal:
process.hgcalLocalRecoSequence = cms.Sequence(
process.HGCalUncalibRecHit+process.HGCalRecHit
#+process.hgcalLayerClusters+process.hgcalMultiClusters+
+process.particleFlowRecHitHGC
+process.particleFlowClusterHGCal)
#+process.particleFlowClusterHGCalFromMultiCl)


#redefininf global reco
process.globalreco = cms.Sequence(process.hbhereco+
process.globalreco_tracking+
process.particleFlowCluster+
process.particleFlowSuperClusterECAL+
process.caloTowersRec+
#process.egammaGlobalReco+
process.recoJets+
process.muonGlobalReco+
#process.pfTrackingGlobalReco+
#process.muoncosmicreco+
process.fastTimingGlobalReco
)


#redefining particleFlowReco sequence
process.particleFlowReco = cms.Sequence(
(process.pfTrack+
process.hgcalTrackCollection+
process.tpClusterProducer+
process.quickTrackAssociatorByHits+
process.simPFProducer)
#+process.particleFlowTrackWithDisplacedVertex
+process.particleFlowBlock+
process.particleFlowTmpBarrel+process.particleFlowTmp)


#process.particleFlowSuperClusteringSequence = cms.Sequence(
#process.particleFlowSuperClusterECAL)
#+process.particleFlowSuperClusterHGCal)
#+process.particleFlowSuperClusterHGCalFromMultiCl)



#Using only the central part to measure the rho corrections 
#process.fixedGridRhoFastjetCentral.pfCandidatesTag = cms.InputTag("particleFlowTmp")
#process.ak4PFL1FastjetCorrector.srcRho = cms.InputTag("fixedGridRhoFastjetCentral")
process.fixedGridRhoFastjetAll.pfCandidatesTag = cms.InputTag("particleFlowTmp")
process.ak4PFL1FastjetCorrector.srcRho = cms.InputTag("fixedGridRhoFastjetAll")


process.ak4PFJetsCorrected = cms.EDProducer("CorrectedPFJetProducer",                                                                                          
    correctors = cms.VInputTag("ak4PFL1FastjetCorrector"),                                                                                                              
    src = cms.InputTag("ak4PFJets")                                                                                                                            
)        
process.ak4PFJetsCHSCorrected = cms.EDProducer("CorrectedPFJetProducer",                                                                                          
    correctors = cms.VInputTag("ak4PFCHSL1FastL2L3Corrector"),                                                                                                              
    src = cms.InputTag("ak4PFJetsCHS")                                                                                                                            
)

process.ak4PFCHSL1FastL2L3Corrector = cms.EDProducer("ChainedJetCorrectorProducer",
    correctors = cms.VInputTag("ak4PFCHSL1FastjetCorrector", "ak4PFCHSL2RelativeCorrector", "ak4PFCHSL3AbsoluteCorrector")
)

process.particleFlowPtrs.src = cms.InputTag("particleFlowTmp")

#redefining reconstruction_step
process.reconstruction = cms.Sequence(process.localreco+
process.globalreco
+process.particleFlowReco
+process.ak4PFJets
+process.fixedGridRhoFastjetAll
+process.ak4PFL1FastjetCorrector
+process.ak4PFJetsCorrected
+process.particleFlowPtrs
+process.goodOfflinePrimaryVertices
+process.pfPileUpJME
+process.pfNoPileUpJME
+process.ak4PFJetsCHS
+process.ak4PFCHSL1FastjetCorrector
+process.ak4PFCHSL2RelativeCorrector
+process.ak4PFCHSL3AbsoluteCorrector
+process.ak4PFCHSL1FastL2L3Corrector
+process.ak4PFJetsCHSCorrected
)

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
#process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)


process.hltOutput = cms.OutputModule( "PoolOutputModule",                                                                                                   
     fileName = cms.untracked.string( "hltoutput_hlt.root" ),                                                                                                         
     fastCloning = cms.untracked.bool( False ),                                                                                                             
     outputCommands = cms.untracked.vstring( 'drop *',                                                                                                      
#         'keep *_particleFlowTmp*_*_*',
         'keep *_ak4PFJets*_*_*',
         'keep *_ak4PFJets*_*_RECO',
         'keep *_ak4GenJets_*_HLT'
                                                                                                                         
         )                                                                                                                                                  
     )                                                                                                                                                      
                                                                                                                                                            
process.HLTOutput = cms.EndPath( process.hltOutput )

# load the DQMStore and DQMRootOutputModule
process.load( "DQMServices.Core.DQMStore_cfi" )
process.DQMStore.enableMultiThread = True


# configure the FastTimerService
process.load( "HLTrigger.Timer.FastTimerService_cfi" )
# print a text summary at the end of the job
process.FastTimerService.printEventSummary         = False
process.FastTimerService.printRunSummary           = False
process.FastTimerService.printJobSummary           = True

# enable DQM plots
process.FastTimerService.enableDQM                 = True

# enable per-path DQM plots (starting with CMSSW 9.2.3-patch2)
process.FastTimerService.enableDQMbyPath           = True

# enable per-module DQM plots
process.FastTimerService.enableDQMbyModule         = True

# enable per-event DQM plots vs lumisection
process.FastTimerService.enableDQMbyLumiSection    = True
process.FastTimerService.dqmLumiSectionsRange      = 2500

# set the time resolution of the DQM plots
process.FastTimerService.dqmTimeRange              = 200000.
process.FastTimerService.dqmTimeResolution         =    20.
process.FastTimerService.dqmPathTimeRange          = 200000.
process.FastTimerService.dqmPathTimeResolution     =    20.
process.FastTimerService.dqmModuleTimeRange        =  200000.
process.FastTimerService.dqmModuleTimeResolution   =     1.

process.FastTimerService.dqmMemoryRange            = 1000000
process.FastTimerService.dqmMemoryResolution       =    5000
process.FastTimerService.dqmPathMemoryRange        = 1000000
process.FastTimerService.dqmPathMemoryResolution   =    5000
process.FastTimerService.dqmModuleMemoryRange      =  100000
process.FastTimerService.dqmModuleMemoryResolution =     500


# set the base DQM folder for the plots
process.FastTimerService.dqmPath                   = 'HLT/TimerService'
process.FastTimerService.enableDQMbyProcesses      = False


##################################### for timing                                                                                                                  
# enable the TrigReport and TimeReport                                                                                                                            
process.options = cms.untracked.PSet(                                                                                                                             
    wantSummary = cms.untracked.bool( True )                                                                                                                      
)                                                                                                                                                                 
process.options.numberOfStreams = cms.untracked.uint32(4)                                                                                                         
process.options.numberOfThreads = cms.untracked.uint32(4) 


# FastTimerService client
process.load('HLTrigger.Timer.fastTimerServiceClient_cfi')
process.fastTimerServiceClient.dqmPath = "HLT/TimerService"


# DQM file saver
process.load('DQMServices.Components.DQMFileSaver_cfi')
process.dqmSaver.workflow = "/HLT/FastTimerService/All"

process.DQMFileSaverOutput = cms.EndPath( process.fastTimerServiceClient + process.dqmSaver )

#process.recosim_step = cms.Path(process.recosim)
#process.endjob_step = cms.EndPath(process.endOfProcess)
# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.recosim_step,process.endjob_step)
#from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
#associatePatAlgosToolsTask(process)

#do not add changes to your config after this point (unless you know what you are doing)
#from FWCore.ParameterSet.Utilities import convertToUnscheduled
#process=convertToUnscheduled(process)


# Customisation from command line

#Have logErrorHarvester wait for the same EDProducers to finish as those providing data for the OutputModule
#from FWCore.Modules.logErrorHarvester_cff import customiseLogErrorHarvesterUsingOutputCommands
#process = customiseLogErrorHarvesterUsingOutputCommands(process)

# Add early deletion of temporary data products to reduce peak memory need
#from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
#process = customiseEarlyDelete(process)
# End adding early deletion

# HLT Tracking v2: re-defining generalTracks as earlyGeneralTracks with only 2 iterations





from SLHCUpgradeSimulations.Configuration.aging import customise_aging_1000
process = customise_aging_1000(process)
