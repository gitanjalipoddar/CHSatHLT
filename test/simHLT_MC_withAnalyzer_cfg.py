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
    input = cms.untracked.int32(20)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/AODSIM/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/BD64F7B5-38EC-B74F-B0EB-D3683FA63832.root',
        '/store/mc/PhaseIITDRSpring19MiniAOD/TTbar_14TeV_TuneCP5_Pythia8/MINIAODSIM/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/B0D41287-E6C9-F841-BC9C-B6C5BE0D2B43.root',
        ##'/store/mc/Run3Summer19DR/TTbar_14TeV_TuneCP5_Pythia8/AODSIM/106X_mcRun3_2024_realistic_v4-v2/270000/24316173-04FE-4848-8B05-3C68E0C29829.root',
        ),
    secondaryFileNames = cms.untracked.vstring(
        ##'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/63D7D147-1CB7-274E-A5FB-D87C2A942004.root',
        '/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/EADD0493-D560-BF4C-8BDE-52D2E29AE588.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/C4CE0C0A-D3F7-9642-9460-6FE89FC0EE3A.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/07E225BE-8790-C64B-BB3B-7FBD56C7AC08.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/1345A820-2449-9E43-810A-CC30953865F3.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/7395F93A-66F3-AA4D-86C9-A04A8FED69F5.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/0C8795A3-EF1D-D449-981B-994DF71D415F.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/C4CE0C0A-D3F7-9642-9460-6FE89FC0EE3A.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/CF0F8489-9CBC-3240-8F07-FF24D439BD3A.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/63D7D147-1CB7-274E-A5FB-D87C2A942004.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/4E1629C4-74D8-894F-967C-806B8E24562F.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/599708A5-F52F-3340-BA0C-44B31A5D6E8B.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/8062FD05-0BE2-F64D-AF71-EF7FD8E72B3E.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/2DC88D55-3191-9E42-9AED-AC34867B8A82.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/7395F93A-66F3-AA4D-86C9-A04A8FED69F5.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/E48ABBC9-01D0-6B49-8919-32AADE090354.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/3AC9C8FB-7464-3C4D-BFD2-7D6FDBC33CC0.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/B4374A34-478F-274C-B88C-D7C9A9A11D4D.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/4E1629C4-74D8-894F-967C-806B8E24562F.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/5E8F45C0-190C-2F4C-ABF9-38E44820D598.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/63D7D147-1CB7-274E-A5FB-D87C2A942004.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/9B28B018-5094-4B4E-801B-A79073FD45E7.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/B4374A34-478F-274C-B88C-D7C9A9A11D4D.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/E0AA2631-0C70-2649-B703-6F95E8F1547A.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/2DC88D55-3191-9E42-9AED-AC34867B8A82.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/07E225BE-8790-C64B-BB3B-7FBD56C7AC08.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/9B28B018-5094-4B4E-801B-A79073FD45E7.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/3AC9C8FB-7464-3C4D-BFD2-7D6FDBC33CC0.root',
'/store/mc/PhaseIITDRSpring19DR/TTbar_14TeV_TuneCP5_Pythia8/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3_ext1-v3/60000/8062FD05-0BE2-F64D-AF71-EF7FD8E72B3E.root',
        )
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

process.globalreco_tracking = cms.Sequence(
	process.offlineBeamSpot+
    process.itLocalReco +
    process.otLocalReco +
    process.calolocalreco +
    process.standalonemuontracking+#we need to add it back for early muons
    process.trackerClusterCheck +
    process.initialStepSequence +
    process.highPtTripletStepSequence +
    process.lowPtQuadStepSequence +
    process.lowPtTripletStepSequence +
    process.detachedQuadStepSequence +
    process.pixelPairStepSequence +
    process.earlyGeneralTracks +
    process.muonSeededStepSequence +
    process.preDuplicateMergingGeneralTracks +
    process.duplicateTrackCandidates +
    process.mergedDuplicateTracks +
    process.duplicateTrackClassifier +
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


process.particleFlowPtrs.src = cms.InputTag("particleFlowTmp")

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

##### PUPPI
process.hltpuppi = process.puppi.clone(
    candName = cms.InputTag("particleFlowTmp"),
    vertexName = cms.InputTag("goodOfflinePrimaryVertices")
)
process.hltak4PFJetsPuppi = process.ak4PFJets.clone(
    src = cms.InputTag("hltpuppi")
    )
process.hltak4PFPUPPIL2RelativeCorrector = cms.EDProducer( "LXXXCorrectorProducer",
    algorithm = cms.string( "AK4PFPuppi" ),
    level = cms.string( "L2Relative" )
)
process.hltak4PFPUPPIL3AbsoluteCorrector= cms.EDProducer( "LXXXCorrectorProducer",
    algorithm = cms.string( "AK4PFPuppi" ),
    level = cms.string( "L3Absolute" )
)
process.hltak4PFPUPPIL2L3Corrector = cms.EDProducer("ChainedJetCorrectorProducer",
    correctors = cms.VInputTag( "hltak4PFPUPPIL2RelativeCorrector", "hltak4PFPUPPIL3AbsoluteCorrector")
)
process.hltak4PFJetsPUPPICorrected = cms.EDProducer("CorrectedPFJetProducer",
    correctors = cms.VInputTag("hltak4PFPUPPIL2L3Corrector"),
    src = cms.InputTag("hltak4PFJetsPuppi")
)


########## Softkiller
process.hltsoftKiller = process.softKiller.clone(
    PFCandidates = cms.InputTag("particleFlowTmp")
)
process.hltak4PFJetsSK = process.ak4PFJetsSK.clone(
    src = cms.InputTag("hltsoftKiller")
)

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
+process.hltpuppi
+process.hltak4PFJetsPuppi
+process.hltak4PFPUPPIL2RelativeCorrector
+process.hltak4PFPUPPIL3AbsoluteCorrector
+process.hltak4PFPUPPIL2L3Corrector
+process.hltak4PFJetsPUPPICorrected
+process.hltsoftKiller
+process.hltak4PFJetsSK
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
         'keep *_hltak4PFJets*_*_*',
         'keep *_ak4PFJets*_*_RECO',
         'keep *_ak4GenJets_*_HLT'

         )
     )

#process.HLTOutput = cms.EndPath( process.hltOutput )


################################################################
################################################################
# Additional output definition
process.TFileService=cms.Service("TFileService",fileName=cms.string( 'simHLTwithAnalyzer_MC.root' ) )

##### Analyzing vertex
#process.hltVerticesComparison = cms.EDAnalyzer('vertexAnalyzer',
#        hltVertex = cms.InputTag("goodOfflinePrimaryVertices::RECO2"),
#        offlineVertex = cms.InputTag("offlinePrimaryVertices"),
#        )
#process.hltVerticesComparison_step = cms.EndPath( process.hltVerticesComparison )

##### Analyzing jets
process.HLTPFCHS = cms.EDAnalyzer('TriggerEfficiencies',
        hltJets = cms.InputTag("ak4PFJetsCHSCorrected::RECO2"),
        recoJets = cms.InputTag("slimmedJetsPuppi::PAT"),
        recojetPt = cms.double( 10 ),
        AK8jets = cms.bool( False ),
        DEBUG = cms.bool(False)
)
process.HLTPFCHS_step = cms.EndPath( process.HLTPFCHS )

process.HLTPFCHSPt20 = process.HLTPFCHS.clone(
        recojetPt = cms.double( 20 ),
)
process.HLTPFCHSPt20_step = cms.EndPath( process.HLTPFCHSPt20 )

process.HLTPFCHSPt30 = process.HLTPFCHS.clone(
        recojetPt = cms.double( 30 ),
)
process.HLTPFCHSPt30_step = cms.EndPath( process.HLTPFCHSPt30 )

process.HLTPFCHSPt40 = process.HLTPFCHS.clone(
        recojetPt = cms.double( 40 ),
)
process.HLTPFCHSPt40_step = cms.EndPath( process.HLTPFCHSPt40 )

process.HLTPFCHSPt50 = process.HLTPFCHS.clone(
        recojetPt = cms.double( 50 ),
)
process.HLTPFCHSPt50_step = cms.EndPath( process.HLTPFCHSPt50 )

process.HLTPFPUPPI = cms.EDAnalyzer('TriggerEfficiencies',
        hltJets = cms.InputTag("hltak4PFJetsPUPPICorrected::RECO2"),
        recoJets = cms.InputTag("slimmedJetsPuppi::PAT"),
        recojetPt = cms.double( 10 ),
        AK8jets = cms.bool( False ),
        DEBUG = cms.bool(False)
)
process.HLTPFPUPPI_step = cms.EndPath( process.HLTPFPUPPI )

process.HLTPFPUPPIPt20 = process.HLTPFPUPPI.clone(
        recojetPt = cms.double( 20 ),
)
process.HLTPFPUPPIPt20_step = cms.EndPath( process.HLTPFPUPPIPt20 )

process.HLTPFPUPPIPt30 = process.HLTPFPUPPI.clone(
        recojetPt = cms.double( 30 ),
)
process.HLTPFPUPPIPt30_step = cms.EndPath( process.HLTPFPUPPIPt30 )

process.HLTPFPUPPIPt40 = process.HLTPFPUPPI.clone(
        recojetPt = cms.double( 40 ),
)
process.HLTPFPUPPIPt40_step = cms.EndPath( process.HLTPFPUPPIPt40 )

process.HLTPFPUPPIPt50 = process.HLTPFPUPPI.clone(
        recojetPt = cms.double( 50 ),
)
process.HLTPFPUPPIPt50_step = cms.EndPath( process.HLTPFPUPPIPt50 )


process.HLTPFSK = cms.EDAnalyzer('TriggerEfficiencies',
        hltJets = cms.InputTag("hltak4PFJetsSK::RECO2"),
        recoJets = cms.InputTag("slimmedJetsPuppi::PAT"),
        recojetPt = cms.double( 10 ),
        AK8jets = cms.bool( False ),
        DEBUG = cms.bool(False)
)
process.HLTPFSK_step = cms.EndPath( process.HLTPFSK )


process.HLTPFSKPt20 = process.HLTPFSK.clone(
        recojetPt = cms.double( 20 ),
)
process.HLTPFSKPt20_step = cms.EndPath( process.HLTPFSKPt20 )

process.HLTPFSKPt30 = process.HLTPFSK.clone(
        recojetPt = cms.double( 30 ),
)
process.HLTPFSKPt30_step = cms.EndPath( process.HLTPFSKPt30 )

process.HLTPFSKPt40 = process.HLTPFSK.clone(
        recojetPt = cms.double( 40 ),
)
process.HLTPFSKPt40_step = cms.EndPath( process.HLTPFSKPt40 )

process.HLTPFSKPt50 = process.HLTPFSK.clone(
        recojetPt = cms.double( 50 ),
)
process.HLTPFSKPt50_step = cms.EndPath( process.HLTPFSKPt50 )

################################################################
################################################################

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
from SLHCUpgradeSimulations.Configuration.aging import customise_aging_1000
process = customise_aging_1000(process)
