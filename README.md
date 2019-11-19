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

### Simulation of HLT paths

In this version, we are simulating the HLT reconstruction with the python config file: [simHLT_MC_withAnalyzer_cfg.py](test/simHLT_MC_withAnalyzer_cfg.py)
