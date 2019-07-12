#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'First argument, name of sample, is needed. Have a good day :)'
else

    sample=$1
    fullTracking=$2
    myproxy=/afs/cern.ch/user/a/algomez/x509up_u15148
    #t3Dir=ttH/training/'${sample}'/
    if [[ $fullTacking -eq 0 ]] ; then
        typeTracking="offlineTracking"
    else
        typeTracking="fullTracking"
    fi

    condorFile=${sample}_condorJobrunStep3_${typeTracking}
    echo '''
universe    =  vanilla
arguments   =  '${fullTracking}' $(myfile) $(ProcId)
executable  =  '${PWD}'/condorlogs/'${condorFile}'.sh
log         =  '${PWD}'/condorlogs/log_'${condorFile}'_$(ClusterId).log
error       =  '${PWD}'/condorlogs/log_'${condorFile}'_$(ClusterId)-$(ProcId).err
output      =  '${PWD}'/condorlogs/log_'${condorFile}'_$(ClusterId)-$(ProcId).out
initialdir  =  '${PWD}'/
getenv      =  True
requirements = (OpSysAndVer =?= "SLCern6")
+JobFlavour = "testmatch"
queue myfile from '${sample}'.txt

    ''' > condorlogs/${condorFile}.sub

    echo '''#!/bin/bash
export SCRAM_ARCH=slc6_amd64_gcc700
export X509_USER_PROXY='${myproxy}'

cd '${CMSSW_BASE}'/src
eval `scramv1 runtime -sh`

echo ${1} ${2} ${3}
cd -
echo "Running: python '${PWD}'/step3_RAW2DIGI_RECO.py fullTracking=${1} inputFile=${2} outputFile=output_${3}"
cmsRun '${PWD}'/step3_RAW2DIGI_RECO.py fullTracking=${1} inputFile=${2} outputFile="output_${3}"  maxEvents=10

    ''' > condorlogs/${condorFile}.sh

    condor_submit condorlogs/${condorFile}.sub

fi
