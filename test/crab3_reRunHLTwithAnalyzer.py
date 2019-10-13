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
config.JobType.psetName = 'reRunHLT_MC_withAnalyzer_cfg.py'
config.JobType.allowUndistributedCMSSW = True
config.JobType.maxMemoryMB = 2500
config.JobType.numCores = 4
#supportFiles = glob.glob('/afs/cern.ch/user/s/sdonato/AFSwork/public/genJetPtHadPU_RunIISummer15GS_ak4GenJetsNoNu/*txt')
#config.JobType.inputFiles = supportFiles
config.General.transferLogs = True

config.Data.inputDataset = ''
#config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader'
config.Data.publication = False
#config.Data.ignoreLocality = True

config.Site.storageSite = 'T2_CH_CSCS'
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
			['/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/RunIISpring18DR-NZSPU0to70_100X_upgrade2018_realistic_v10-v1/AODSIM', '/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/RunIISpring18DR-NZSPU0to70_100X_upgrade2018_realistic_v10-v1/GEN-SIM-RAW'],
			]


	from multiprocessing import Process
	for dataset in Samples:
		config.Data.inputDataset = dataset[0]
		config.Data.secondaryInputDataset = dataset[1]
		procName = dataset[0].split('/')[1]+'_TriggerStudies_'+version
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
