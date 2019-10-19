##### Remember to do ls with the following command
#####gfal-ls srm://storage01.lcg.cscs.ch:8443/srm/managerv2?SFN=/pnfs/lcg.cscs.ch/cms/trivcat/store/user/gpoddar/PUatHLT/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/crab_QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8_TriggerStudies_v01/190902_094522/0000/

prefix=srm://storage01.lcg.cscs.ch:8443/srm/managerv2?SFN=/pnfs/lcg.cscs.ch/cms/trivcat/store/user/gpoddar/
folder=PUatHLT/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/crab_QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8_TriggerStudies_v01/191010_092250/0000/
nameOutput='reRunHLTwithAnalyzer_MC.root'

mkdir -p tmpFolder/

for ifile in `gfal-ls ${prefix}/${folder}`; do
    if [[ $ifile == *root ]]; then
        echo "gfal-copy --force ${prefix}/${folder}/${ifile} file://${PWD}/tmpFolder/${ifile}"
        gfal-copy --force ${prefix}/${folder}/${ifile} file://${PWD}/tmpFolder/${ifile}
    fi
done

cd tmpFolder/
hadd -f ../${nameOutput} *root
cd ../
rm -rf tmpFolder/
echo "Have a nice day :D"
