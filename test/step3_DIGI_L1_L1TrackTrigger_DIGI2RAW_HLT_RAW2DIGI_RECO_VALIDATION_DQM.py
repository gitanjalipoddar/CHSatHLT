# Auto generated configuration file
# using:
# Revision: 1.19
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v
# with command line options: step3_withDQM --conditions auto:phase1_2018_realistic -n 10 --era Run2_2018 --eventcontent RECOSIM,DQM --runUnscheduled -s DIGI:pdigi_valid,L1,L1TrackTrigger,DIGI2RAW,HLT:@fake2,RAW2DIGI,RECO:reconstruction_trackingOnly,VALIDATION:@trackingOnlyValidation,DQM:@trackingOnlyDQM --datatier GEN-SIM-RECO,DQMIO --geometry DB:Extended --filein /store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/30000/26371D77-1622-E811-8681-0242AC130002.root --fileout file:output_offlineTracking.root --process HLT2
import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('python')
options.register('inputFile',
		'/store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/30000/26371D77-1622-E811-8681-0242AC130002.root',
		VarParsing.multiplicity.singleton,
		VarParsing.varType.string,
		"Input file"
)
options.register('outputFileName',
		'output',
		VarParsing.multiplicity.singleton,
		VarParsing.varType.string,
		"Input file"
)
options.register('fullTracking',
		False,
		VarParsing.multiplicity.singleton,
		VarParsing.varType.bool,
		"Running full tracking or subset"
)
options.parseArguments()


from Configuration.StandardSequences.Eras import eras

process = cms.Process('HLT2',eras.Run2_2018)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.L1TrackTrigger_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_Fake2_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.Validation_cff')
process.load('DQMOffline.Configuration.DQMOfflineMC_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring([ options.inputFile ]),
    inputCommands = cms.untracked.vstring(
        'keep *',
        'drop *_genParticles_*_*',
        'drop *_genParticlesForJets_*_*',
        'drop *_kt4GenJets_*_*',
        'drop *_kt6GenJets_*_*',
        'drop *_iterativeCone5GenJets_*_*',
        'drop *_ak4GenJets_*_*',
        'drop *_ak7GenJets_*_*',
        'drop *_ak8GenJets_*_*',
        'drop *_ak4GenJetsNoNu_*_*',
        'drop *_ak8GenJetsNoNu_*_*',
        'drop *_genCandidatesForMET_*_*',
        'drop *_genParticlesForMETAllVisible_*_*',
        'drop *_genMetCalo_*_*',
        'drop *_genMetCaloAndNonPrompt_*_*',
        'drop *_genMetTrue_*_*',
        'drop *_genMetIC5GenJs_*_*'
    ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step3_withDQM nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition
if options.fullTracking: outputName = options.outputFileName+"_offlineTracking.root"
else: outputName = options.outputFileName+"_onlineTracking.root"
process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RECO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:'+outputName),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('DQMIO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:'+outputName.replace('.root', '_inDQM.root')),
    outputCommands = process.DQMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

if not options.fullTracking:
    print "|----- NOT RUNNING FULL TRACKING"
    process.lowPtTripletStepStandardTrajectoryFilter.minimumNumberOfHits = 10000
    process.detachedQuadStepTrajectoryFilterBase.minimumNumberOfHits = 10000
    process.detachedTripletStepTrajectoryFilterBase.minimumNumberOfHits = 10000
    process.pixelPairStepTrajectoryFilterBase.minimumNumberOfHits = 10000
    process.mixedTripletStepTrajectoryFilter.minimumNumberOfHits = 10000
    process.pixelLessStepTrajectoryFilter.minimumNumberOfHits = 10000
    process.tobTecStepTrajectoryFilter.minimumNumberOfHits = 10000
    process.jetCoreRegionalStepTrajectoryFilter.minimumNumberOfHits = 10000
    #process.muonSeededStepInOutTrajectoryFilter.minimumNumberOfHits = 10000
    #process.muonSeededStepOutInTrajectoryFilter.minimumNumberOfHits = 10000
else:
    print "|----- RUNNING FULL TRACKING"

# Other statements
process.mix.digitizers = cms.PSet(process.theDigitizersValid)
from HLTrigger.Configuration.CustomConfigs import ProcessName
process = ProcessName(process)

from Configuration.Applications.ConfigBuilder import ConfigBuilder
process.globalValidationTrackingOnly.visit(ConfigBuilder.MassSearchReplaceProcessNameVisitor("HLT", "HLT2", whitelist = ("subSystemFolder",)))
process.globalPrevalidationTrackingOnly.visit(ConfigBuilder.MassSearchReplaceProcessNameVisitor("HLT", "HLT2", whitelist = ("subSystemFolder",)))
process.DQMOfflineTracking.visit(ConfigBuilder.MassSearchReplaceProcessNameVisitor("HLT", "HLT2", whitelist = ("subSystemFolder",)))
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic', '')

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi_valid)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.L1TrackTrigger_step = cms.Path(process.L1TrackTrigger)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction_trackingOnly)
process.prevalidation_step = cms.Path(process.globalPrevalidationTrackingOnly)
process.validation_step = cms.EndPath(process.globalValidationTrackingOnly)
process.dqmoffline_step = cms.EndPath(process.DQMOfflineTracking)
process.dqmofflineOnPAT_step = cms.EndPath(process.PostDQMOffline)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)
process.DQMoutput_step = cms.EndPath(process.DQMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.L1TrackTrigger_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.raw2digi_step,process.reconstruction_step,process.prevalidation_step,process.validation_step,process.dqmoffline_step,process.dqmofflineOnPAT_step,process.RECOSIMoutput_step,process.DQMoutput_step])
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforMC(process)

# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)


# Customisation from command line

#Have logErrorHarvester wait for the same EDProducers to finish as those providing data for the OutputModule
from FWCore.Modules.logErrorHarvester_cff import customiseLogErrorHarvesterUsingOutputCommands
process = customiseLogErrorHarvesterUsingOutputCommands(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
