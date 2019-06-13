# CHSatHLT

### To set initial environment

This is different in every branch. Please check that the name of the branch corresponds to the CMSSW release.

```
cmsrel CMSSW_10_1_11_patch1
cd CMSSW_10_1_11_patch1/src
cmsenv
git cms-addpkg HLTrigger/Configuration          ### From HLT recommendations, not used yet
git cms-addpkg RecoTracker/IterativeTracking    ### For tracking sequence
git cms-addpkg RecoVertex/V0Producer            ### From Tracker recommendations, not used yet
git clone git@github.com:alefisico/CHSatHLT.git -b 101X PUHLT/CHSatHLT
scram b -j 8 
```

### Run different tracking offline

#### Intro

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

We want to run only the tracking recontruction. Therefore we can use the following commands to create config files for this:

```
runTheMatrix.py -n | grep 2018 | grep trackingOnly
runTheMatrix.py -l 10824.1 --dryRun
```

the first one is to check the number of the process created by the runTheMatrix command. The second takes that number (10824.1) and runs only that sequence. The important part is step3. A modified cmsDriver command for running RAW datasets is:

```
cmsDriver.py step3 --conditions auto:phase1_2018_realistic -n 10 --era Run2_2018 --eventcontent RECOSIM --runUnscheduled -s RAW2DIGI,RECO:reconstruction_trackingOnly --datatier GEN-SIM-RECO --geometry DB:Extended --filein file:step2.root --fileout file:step3.root
```

Which creates the file `PUHLT/CHSatHLT/test/step3_RAW2DIGI_RECO.py`. You can use this file directly. This file is modified to include the full tracking reconstructions or just the first three steps (as described in intro).  To run it:

```
cd $CMSSW_BASE/src/PUHLT/CHSatHLT/test/
cmsRun step3_RAW2DIGI_RECO.py maxEvents=100 fullTracking=True
```

##### To run condor job

In addition, the files `condor_runStep3.sub` and `condor_runStep3.sh` can be used to run condor jobs. Modified them with your information and then to submit a job use `condor_submit condor_runStep3.sub` and to check status `condor_q`. 
