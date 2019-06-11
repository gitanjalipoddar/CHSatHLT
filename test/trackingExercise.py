##### Taken from https://github.com/cms-sw/cmssw/blob/02d4198c0b6615287fd88e9a8ff650aea994412e/SimTracker/SiPixelDigitizer/test/runDigisToTracks.py
import FWCore.ParameterSet.Config as cms
process = cms.Process("RP")
process.load("CondCore.CondDB.CondDB_cfi")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load('Configuration.StandardSequences.RecoSim_cff')
#process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '100X_upgrade2018_realistic_v10', '')

# clusterizer
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

# for raw
#process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi")
#process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')

# needed for pixel RecHits (templates?)
process.load("Configuration.StandardSequences.Reconstruction_cff")
#process.load("RecoTracker.Configuration.RecoTracker_cff")

# iterative tracking
process.load("RecoTracker.IterativeTracking.iterativeTk_cff")

## for Raw2digi for simulations
#process.siPixelDigis.InputLabel = 'siPixelRawData'
#process.siStripDigis.ProductLabel = 'SiStripDigiToRaw'
## for digi to clu
#process.siPixelClusters.src = cms.InputTag('siPixelDigis')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/30000/26371D77-1622-E811-8681-0242AC130002.root'),
)

process.RECO = cms.OutputModule("PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('recoTracks.root')
)

#process.p1 = cms.Path(process.RawToDigi*process.trackerlocalreco*process.offlineBeamSpot*process.ckftracks)
#process.p1 = cms.Path(process.RawToDigi*process.SiPixelClusterShapeCache*process.trackerlocalreco*process.offlineBeamSpot*process.iterTracking)
#process.p1 = cms.Path(process.siPixelRawData*process.SiStripDigiToRaw*process.siPixelDigis*process.siStripDigis*process.trackerlocalreco*process.offlineBeamSpot*process.siPixelClusterShapeCache*process.recopixelvertexing*process.MeasurementTrackerEvent*process.myTracking*process.vertexreco)
process.p1 = cms.Path( process.siPixelClusterShapeCache * process.iterTracking )
#process.p1 = cms.Path( process.RawToDigi
#            * process.siPixelRawData
#            * process.SiStripDigiToRaw
#            * process.siPixelDigis
#            * process.siStripDigis
#            * process.trackerlocalreco
#            * process.offlineBeamSpot
#            * process.siPixelClusterShapeCache
#            * process.recopixelvertexing
#            * process.MeasurementTrackerEvent
#            * process.iterTracking
#            * process.vertexreco)
process.outpath = cms.EndPath(process.RECO)
