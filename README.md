# CHSatHLT


## Run different tracking offline

### Intro

To quickly compare offline/online tracking, we can just turn off some of the offline tracking steps. 

Offline tracking makes use of the following steps ((https://cmssdt.cern.ch/dxr/CMSSW/source/RecoTracker/IterativeTracking/python/iterativeTkConfig.py)[config file]):

```
_iterations_trackingPhase1 = [
    "InitialStep",
    "LowPtQuadStep",
    "HighPtTripletStep",
    "LowPtTripletStep",
    "DetachedQuadStep",
    "DetachedTripletStep",
    "PixelPairStep",
    "MixedTripletStep",
    "PixelLessStep",
    "TobTecStep",
    "JetCoreRegionalStep",
]
+

_iterations_muonSeeded = [
    "MuonSeededStepInOut",
    "MuonSeededStepOutIn",
]
```

while the online tracking uses
```
cms.Sequence( fragment.HLTIterativeTrackingIteration0 + fragment.HLTIterativeTrackingIteration1 + fragment.hltIter1Merged + fragment.HLTIter1TrackAndTauJets4Iter2Sequence + fragment.HLTIterativeTrackingIteration2 + fragment.hltIter2Merged + fragment.HLTIterativeTrackingDoubletRecovery + fragment.hltMergedTracks )
```

then:
  * hltIter0 should be similar to InitialStep
  * hltIter1 should be similar to LowPtQuadStep
  * hltIter2 should be similar to HighPtTripletStep

Full set of instructions on how to remove tracking sequences are (https://cmssdt.cern.ch/dxr/CMSSW/source/RecoTracker/IterativeTracking/README.md)[here]

#### To run reconstruction with different tracking
