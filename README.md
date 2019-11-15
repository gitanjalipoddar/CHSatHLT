# PileUp Mitigation at HLT

### To set initial environment

This is different in every branch. Please check that the name of the branch corresponds to the CMSSW release.

```
cmsrel CMSSW_11_0_0_pre7
cd CMSSW_11_0_0_pre7/src
cmsenv
git cms-addpkg HLTrigger/Configuration          ### From HLT recommendations, not used yet
git clone git@github.com:cms-jet/JetToolbox.git JMEAnalysis/JetToolbox -b jetToolbox_102X_v2
git clone git@github.com:alefisico/PUmitigationatHLT.git -b 110X PUHLT/PUmitigationatHLT
scram b -j 8 
```


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
