# Auto generated configuration file
# using:
# Revision: 1.19
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v
# with command line options: reRunHLT --filein /store/data/Run2018D/HLTPhysics/RAW/v1/000/320/571/00000/0ECA1B77-9894-E811-BFE4-FA163EED3BED.root --fileout reRunHLT.root --data --step HLT --era Run2_2018 --processName HLT2 -n 100 --conditions auto:run2_hlt_GRun
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('HLT2',eras.Run2_2018)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
#process.load('HLTrigger.Configuration.HLT_GRun_cff')
process.load('PUHLT.PUmitigationatHLT.HLT_User_cff')   ##### DONT FORGET TO CHANGE THIS
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/data/Run2018D/HLTPhysics2/AOD/PromptReco-v2/000/323/395/00000/889188C8-C7C6-E648-8288-543C06AF8F89.root'
        ),
    secondaryFileNames = cms.untracked.vstring(
        '/store/data/Run2018D/HLTPhysics2/RAW/v1/000/323/395/00000/95D69306-A42F-D641-A190-60018F799057.root'
    ),
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool( True ),
    numberOfThreads = cms.untracked.uint32( 4 ),
    numberOfStreams = cms.untracked.uint32( 0 ),
    sizeOfStackForThreadsInKB = cms.untracked.uint32( 10*1024 )
)


# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('reRunHLT nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('reRunHLTwithAnalyzer.root'),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition
from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
jetToolbox( process, 'ak4', 'jetSequence', 'noOutput', dataTier='AOD', runOnMC=False, PUMethod='Puppi' ) #, verbosity=8)

# Additional output definition
process.TFileService=cms.Service("TFileService",fileName=cms.string( 'reRunHLTwithAnalyzer.root' ) )

##### Analyzing vertex
process.hltPixelVerticesComparison = cms.EDAnalyzer('vertexAnalyzer',
        hltVertex = cms.InputTag("hltPixelVertices"),
        offlineVertex = cms.InputTag("offlinePrimaryVertices"),
        )
process.hltPixelVerticesComparison_step = cms.EndPath( process.hltPixelVerticesComparison )

process.hltVerticesPFComparison = process.hltPixelVerticesComparison.clone(
        hltVertex = cms.InputTag("hltVerticesPF"),
        )
process.hltVerticesPFComparison_step = cms.EndPath( process.hltVerticesPFComparison )

process.hltVerticesPFSelectorComparison = process.hltPixelVerticesComparison.clone(
        hltVertex = cms.InputTag("hltVerticesPFSelector"),
        )
process.hltVerticesPFSelectorComparison_step = cms.EndPath( process.hltVerticesPFSelectorComparison )


##### Analyzing jets
process.HLTPFHT = cms.EDAnalyzer('TriggerEfficiencies',
        bits = cms.InputTag("TriggerResults::HLT2"),
        objects = cms.InputTag("hltAK4PFJetsCorrected::HLT2"),
        baseTrigger = cms.string("HLT_PFHTNoThreshold"),
        triggerPass = cms.vstring([ "HLT_PFHTNoThreshold" ] ),
        recoJets = cms.InputTag("ak4PFJetsCHS"),
        patJets = cms.InputTag("ak4PFJetsPuppi"),
        #patJets = cms.InputTag("patJetsAK4PFCHSPATJetswithUserData::HLT2"),
        recojetPt = cms.double( 10 ),
        AK8jets = cms.bool( False ),
        DEBUG = cms.bool(False)
)
process.HLTPFHT_step = cms.EndPath( process.HLTPFHT )

process.HLTPFCHSHT = process.HLTPFHT.clone(
        objects = cms.InputTag("hltAK4PFJetsForCHS::HLT2"),
        baseTrigger = cms.string("HLT_PFCHSHTNoThreshold"),
        triggerPass = cms.vstring([ "HLT_PFCHSHTNoThreshold" ] ),
        )
process.HLTPFCHSHT_step = cms.EndPath( process.HLTPFCHSHT )

process.HLTPFCHSVerticesPFHT = process.HLTPFHT.clone(
        objects = cms.InputTag("hltAK4PFJetsForCHSVerticesPF::HLT2"),
        baseTrigger = cms.string("HLT_PFCHSHTNoThreshold"),
        triggerPass = cms.vstring([ "HLT_PFCHSHTNoThreshold" ] ),
        )
process.HLTPFCHSVerticesPFHT_step = cms.EndPath( process.HLTPFCHSVerticesPFHT )

process.HLTPFSKHT = process.HLTPFHT.clone(
        objects = cms.InputTag("hltAK4PFJetsForSK::HLT2"),
        baseTrigger = cms.string("HLT_PFSKHTNoThreshold"),
        triggerPass = cms.vstring([ "HLT_PFSKHTNoThreshold" ] ),
        )
process.HLTPFSKHT_step = cms.EndPath( process.HLTPFSKHT )

process.HLTPFPuppiHT = process.HLTPFHT.clone(
        objects = cms.InputTag("hltAK4PFJetsForPuppi::HLT2"),
        baseTrigger = cms.string("HLT_PFPuppiHTNoThreshold"),
        triggerPass = cms.vstring([ "HLT_PFPuppiHTNoThreshold" ] ),
        )
process.HLTPFPuppiHT_step = cms.EndPath( process.HLTPFPuppiHT )


# Other statements
from HLTrigger.Configuration.CustomConfigs import ProcessName
process = ProcessName(process)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_hlt_GRun', '')

# Path and EndPath definitions
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule()
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.hltPixelVerticesComparison_step,process.hltVerticesPFComparison_step, process.hltVerticesPFSelectorComparison_step])
process.schedule.extend([process.HLTPFHT_step,process.HLTPFCHSHT_step,process.HLTPFCHSVerticesPFHT_step,process.HLTPFSKHT_step,process.HLTPFPuppiHT_step])
process.schedule.extend([process.endjob_step])
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
