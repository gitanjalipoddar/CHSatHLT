##################################################################
########   TO RUN THIS: python crab3_QCD.py
########   DO NOT DO: crab submit crab3_QCD.py
##################################################################

from CRABClient.UserUtilities import config
from httplib import HTTPException
import glob, os

config = config()

version = 'v01'

config.General.requestName = ''
config.General.workArea = 'crab_projects'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'simHLT_MC_withAnalyzer_cfg.py'
config.JobType.allowUndistributedCMSSW = True
config.JobType.maxMemoryMB = 6000
config.JobType.numCores = 4
#supportFiles = glob.glob('/afs/cern.ch/user/s/sdonato/AFSwork/public/genJetPtHadPU_RunIISummer15GS_ak4GenJetsNoNu/*txt')
#config.JobType.inputFiles = supportFiles
config.General.transferLogs = True

config.Data.inputDataset = ''
#config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader'
config.Data.publication = False
#config.Data.ignoreLocality = True

config.Site.storageSite = 'T3_CH_PSI'
config.Data.outLFNDirBase = '/store/user/'+os.environ['USER']+'/PUatHLT/'

def submit(config):
    try:
        #crabCommand('submit', '--dryrun', config = config)
        crabCommand('submit', config = config)
    except HTTPException, hte:
        print 'Cannot execute commend'
        print hte.headers

if __name__ == '__main__':

	from CRABAPI.RawCommand import crabCommand

	Samples = [
			['/TTbar_14TeV_TuneCP5_Pythia8/PhaseIITDRSpring19MiniAOD-NoPU_106X_upgrade2023_realistic_v3-v1/MINIAODSIM', '/TTbar_14TeV_TuneCP5_Pythia8/PhaseIITDRSpring19DR-NoPU_106X_upgrade2023_realistic_v3-v1/GEN-SIM-DIGI-RAW'],
                        ['/TTbar_14TeV_TuneCP5_Pythia8/PhaseIITDRSpring19MiniAOD-PU200_106X_upgrade2023_realistic_v3_ext1-v3/MINIAODSIM', '/TTbar_14TeV_TuneCP5_Pythia8/PhaseIITDRSpring19DR-PU200_106X_upgrade2023_realistic_v3_ext1-v3/GEN-SIM-DIGI-RAW']
			]


	from multiprocessing import Process
	for dataset in Samples:
		config.Data.inputDataset = dataset[0]
		config.Data.secondaryInputDataset = dataset[1]
		procName = dataset[0].split('/')[1]+"_"+dataset[0].split('/')[2].split('_real')[0]+'_TriggerStudies_'+version
		#config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt'
		config.Data.splitting = 'FileBased' # 'FileBased'
		#config.JobType.outputFiles = ['hltbits.root']
		#config.JobType.maxMemoryMB = 2500
		config.Data.unitsPerJob = 1

		config.General.requestName = procName
		print config
		p = Process(target=submit, args=(config,))
		p.start()
		p.join()
