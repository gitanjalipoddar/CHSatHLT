#!/usr/bin/env sh
export X509_USER_PROXY=/afs/cern.ch/user/a/algomez/x509up_u15148

source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc630
cd /afs/cern.ch/work/a/algomez/triggerStudies/CMSSW_10_1_11_patch1/src/
eval `scram runtime -sh`

cmsRun hlt_CHS_data_v1.py
