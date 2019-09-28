# Auto generated configuration file
# using:
# Revision: 1.19
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v
# with command line options: reRunHLT_TEST --filein /store/data/Run2018D/HLTPhysics/RAW/v1/000/320/571/00000/0ECA1B77-9894-E811-BFE4-FA163EED3BED.root --fileout reRunHLT.root --data --step HLT:User --era Run2_2018 --processName HLT2 -n 100 --conditions auto:run2_data_GRun
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
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#        '/store/data/Run2018D/HLTPhysics/MINIAOD/PromptReco-v2/000/320/571/00000/D0D07B05-5396-E811-8EB1-FA163EEE2BDE.root',
#	'/store/data/Run2018D/HLTPhysics/MINIAOD/PromptReco-v1/000/320/448/00000/E8215677-1894-E811-9C82-02163E00C230.root',
#	'/store/data/Run2018D/HLTPhysics/MINIAOD/PromptReco-v1/000/320/472/00000/9ECEF7B6-BF94-E811-AE08-FA163E991BA6.root',
#	'/store/data/Run2018D/HLTPhysics/MINIAOD/PromptReco-v1/000/320/433/00000/7E4FD255-9693-E811-96EC-02163E010C78.root',
#	'/store/data/Run2018D/HLTPhysics/MINIAOD/PromptReco-v1/000/320/477/00000/A4B40067-D994-E811-AEDA-FA163E7C898A.root',
#	'/store/data/Run2018D/HLTPhysics/MINIAOD/PromptReco-v1/000/320/476/00000/48BE1B42-DB94-E811-870F-FA163E6D9F0E.root',
#	'/store/data/Run2018D/HLTPhysics/MINIAOD/PromptReco-v1/000/320/435/00000/D02CA6C7-B793-E811-8A99-02163E016490.root',
        '/store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/AODSIM/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/30000/1EAD2926-BA22-E811-B2D3-0242AC130002.root'
        ),
    secondaryFileNames = cms.untracked.vstring(
#        '/store/data/Run2018D/HLTPhysics/RAW/v1/000/320/571/00000/0ECA1B77-9894-E811-BFE4-FA163EED3BED.root',
#        '/store/data/Run2018D/HLTPhysics/RAW/v1/000/320/448/00000/B41AE727-9692-E811-9378-FA163EBFAD68.root',
#        '/store/data/Run2018D/HLTPhysics/RAW/v1/000/320/472/00000/F4C3F921-3693-E811-A4F9-FA163E1377DD.root',
#        '/store/data/Run2018D/HLTPhysics/RAW/v1/000/320/433/00000/3C0F5554-0792-E811-9C35-FA163E467136.root',
#        '/store/data/Run2018D/HLTPhysics/RAW/v1/000/320/477/00000/10E7DD5C-4B93-E811-A142-FA163E7B68E6.root',
#        '/store/data/Run2018D/HLTPhysics/RAW/v1/000/320/476/00000/2C6197B2-5593-E811-B404-FA163EDB8EB7.root',
#        '/store/data/Run2018D/HLTPhysics/RAW/v1/000/320/435/00000/F0CC2975-2792-E811-8AE6-02163E019EA3.root'
	'/store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/30000/0AF1577C-B522-E811-AD72-0242AC130002.root',
	'/store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/30000/2C12B1F8-7E22-E811-8452-0242AC130002.root',
	'/store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/30000/2E603D06-2722-E811-A97C-0242AC130002.root',
	'/store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/30000/C2E5A79F-F021-E811-8233-0242AC130002.root',
	'/store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/30000/FAE43C1C-1122-E811-A81C-0242AC130002.root',
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
    annotation = cms.untracked.string('reRunHLT_TEST nevts:100'),
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
    outputCommands = cms.untracked.vstring("keep *"),  #process.RECOSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition
process.TFileService=cms.Service("TFileService",fileName=cms.string( 'reRunHLTwithAnalyzer.root' ) )

process.PFHTTriggerEfficiency = cms.EDAnalyzer('TriggerEfficiencies',
        bits = cms.InputTag("TriggerResults::HLT2"),
        objects = cms.InputTag("hltAK4PFJets::HLT2"),
        baseTrigger = cms.string("HLT_SingleMu24"),
        triggerPass = cms.vstring([ "HLT_PFHT1050" ] ),
        recoJets = cms.InputTag("slimmedJets"),
        recojetPt = cms.double( 10 ),
        AK8jets = cms.bool( False ),
        DEBUG = cms.bool(True)
)
process.PFHTTriggerEfficiency_step = cms.EndPath( process.PFHTTriggerEfficiency )

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
#process.schedule.extend([process.endjob_step])
process.schedule.extend([process.PFHTTriggerEfficiency_step])
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion

if 'MessageLogger' in process.__dict__:
    process.MessageLogger.categories.append('TriggerSummaryProducerAOD')
    process.MessageLogger.categories.append('L1GtTrigReport')
    process.MessageLogger.categories.append('L1TGlobalSummary')
    process.MessageLogger.categories.append('HLTrigReport')
    process.MessageLogger.categories.append('FastReport')
