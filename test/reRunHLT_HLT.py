# Auto generated configuration file
# using:
# Revision: 1.19
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v
# with command line options: reRunHLT --filein /store/data/Run2018D/HLTPhysics/RAW/v1/000/320/571/00000/0ECA1B77-9894-E811-BFE4-FA163EED3BED.root --fileout reRunHLT.root --data --step HLT:User --era Run2_2018 --processName HLT2 -n 10 --conditions auto:run2_data_GRun
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
process.load('HLTrigger.Configuration.HLT_User_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('/store/data/Run2018D/HLTPhysics/RAW/v1/000/320/571/00000/0ECA1B77-9894-E811-BFE4-FA163EED3BED.root'),
    fileNames = cms.untracked.vstring('/store/data/Run2018D/HLTPhysics/MINIAOD/PromptReco-v2/000/325/210/00000/7A80BD19-95C9-834C-B414-8DE6C57C61C4.root'),
    secondaryFileNames = cms.untracked.vstring('/store/data/Run2018D/HLTPhysics/RAW/v1/000/325/210/00000/696C3DB1-6DE4-9C46-99EC-CF7E03A1FA7C.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('reRunHLT nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('reRunHLT.root'),
    #outputCommands = process.RECOSIMEventContent.outputCommands,
    outputCommands = cms.untracked.vstring( 'keep *' ),
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from HLTrigger.Configuration.CustomConfigs import ProcessName
process = ProcessName(process)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data_GRun', '')

# Path and EndPath definitions
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule()
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.RECOSIMoutput_step])
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)


process.TFileService=cms.Service("TFileService",fileName=cms.string( 'reRunHLTwithAnalyzerv2.root' ) )

process.PFHTTriggerEfficiency = cms.EDAnalyzer('TriggerValidationAndEfficiencies',
		baseTrigger = cms.string("empty"),
		triggerPass = cms.vstring([ "HLT_PFHT1050_v" ] ),
		recoJets = cms.InputTag("slimmedJets"),
		recojetPt = cms.double( 30 ),
		AK8jets = cms.bool( False ),
		objects = cms.InputTag("selectedPatTrigger")
)
process.PFHTTriggerEfficiency_step = cms.EndPath( process.PFHTTriggerEfficiency )# End adding early deletion
