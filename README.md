# CHSatHLT

### To set initial environment

This is different in every branch. Please check that the name of the branch corresponds to the CMSSW release.

```
cmsrel CMSSW_10_2_15
cd CMSSW_10_2_15/src
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

Offline tracking makes use of the following steps ([config file](https://cmssdt.cern.ch/dxr/CMSSW/source/RecoTracker/IterativeTracking/python/iterativeTkConfig.py)):

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

Full set of instructions on how to remove tracking sequences are [here](https://cmssdt.cern.ch/dxr/CMSSW/source/RecoTracker/IterativeTracking/README.md)

#### To run reconstruction with different tracking

We want to run only the tracking recontruction. Therefore we can use the following commands to create config files for this:

```
runTheMatrix.py -n | grep 2018 | grep trackingOnly
runTheMatrix.py -l 10824.1 --dryRun
```

the first one is to check the number of the process created by the runTheMatrix command. The second takes that number (10824.1) and runs only that sequence. The important part is step3. A modified cmsDriver command for running RAW datasets is:

```
cmsDriver.py step3_withDQM  --conditions auto:phase1_2018_realistic -n 10 --era Run2_2018 --eventcontent RECOSIM,DQM --runUnscheduled  -s DIGI:pdigi_valid,L1,L1TrackTrigger,DIGI2RAW,HLT:@fake2,RAW2DIGI,RECO:reconstruction_trackingOnly,VALIDATION:@trackingOnlyValidation,DQM:@trackingOnlyDQM --datatier GEN-SIM-RECO,DQMIO --geometry DB:Extended --filein  /store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/30000/26371D77-1622-E811-8681-0242AC130002.root  --fileout file:output_offlineTracking.root  --process HLT2
```

Which creates the file `PUHLT/CHSatHLT/test/step3_DIGI_L1_L1TrackTrigger_DIGI2RAW_HLT_RAW2DIGI_RECO_VALIDATION_DQM.py`. You can use this file directly. This file is modified to include the full tracking reconstructions or just the first three steps (as described in intro).  To run it:

```
cd $CMSSW_BASE/src/PUHLT/CHSatHLT/test/
cmsRun step3_DIGI_L1_L1TrackTrigger_DIGI2RAW_HLT_RAW2DIGI_RECO_VALIDATION_DQM.py maxEvents=100 fullTracking=True
```

This step creates two root files, for instance `output_onlineTracking.root` and `output_onlineTracking_inDQM.root`. The second file (the one which ends in `inDQM`) is needed for the step4, which creates DQM plots from this output. To get the file to run this step:

```
cmsDriver.py step4  --conditions auto:phase1_2018_realistic -s HARVESTING:@trackingOnlyValidation+@trackingOnlyDQM --scenario pp --filetype DQM --geometry DB:Extended --era Run2_2018 --mc  -n 100  --filein file:output_offlineTracking_inDQM.root --fileout file:step4.root
```

which creates the file called `step4_HARVESTING.py`. These file can be run directly with cmsRun, it only needs to change the input file.

##### To run condor job

In addition, the file `submitCondorJobs_runStep3.sh` can be used to run condor jobs. It needs a text file with the names of the root files to run and can be run as:
```
source submitCondorJobs_runStep3.sh QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8 0
```
where the first parameter corresponds to the name of the txt file (without .txt) and the second parameter defines if running full offline Tracking (0) or partial HLT-like tracking (1)
Dont forget to modify your proxy with your information and to check status `condor_q`. 

### Re-run HLT and analyze output

The general idea is to compare the possible HLT thresholds with respect to offline reconstructed jets. First, a new HLT menu has to be created using the [confdb gui](https://twiki.cern.ch/twiki/bin/viewauth/CMS/EvfConfDBGUI). 

Once the HLT menu is created, following the steps from the [twiki page](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideGlobalHLT#Dumping_the_latest_HLT_menu_conf), dump the config-fragment file for running with cmsDriver.py:
~~~
hltGetConfiguration --cff --offline /users/algomez/PUatHLT/V12 --setup /dev/CMSSW_10_1_0/GRun > HLT_User_cff.py
~~~

Following the instructions in the twiki, this file `HLT_User_cff.py` has to be moved to `$CMSSW_BASE/src/HLTrigger/Configuration/python/`.

Then, to rerun HLT in a RAW file, we need to run:
~~~
cmsDriver.py reRunHLT_MC --filein /store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/30000/0AF1577C-B522-E811-AD72-0242AC130002.root --fileout reRunHLT_MC.root --step HLT:User --era Run2_2018 --processName HLT2 -n 100 --conditions auto:run2_mc_GRun
~~~
this command will create a file called `reRunHLT_MC.py` which will run the new HLT user menu alone. 

Finally, we can analyze the output of the new HLT step with some AOD/miniAOD files. For that we need to modify the `reRunHLT_MC.py` as shown in [reRunHLT_withAnalyzer_cfg.py](test/reRunHLT_withAnalyzer_cfg.py).
