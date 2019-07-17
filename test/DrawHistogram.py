#!/usr/bin/env python
'''
File: DrawHistogram.py
Author: Alejandro Gomez Espinosa
Email: gomez@physics.rutgers.edu
Description: My Draw histograms. Check for options at the end.
'''

from ROOT import *
import time, os, math, sys
import argparse
import B2GTriggerStudies.TriggerEfficiencies.CMS_lumi as CMS_lumi 
import B2GTriggerStudies.TriggerEfficiencies.tdrstyle as tdrstyle

#gROOT.Reset()
gROOT.SetBatch()
gROOT.ForceStyle()
tdrstyle.setTDRStyle()

gStyle.SetOptStat(0)


def plotTriggerEfficiency( inFileSample, sample, triggerDenom, name, cut, xmin, xmax, xlabel, rebin, labX, labY, log):
	"""docstring for plot"""

	outputFileName = name+'_'+cut+'_'+triggerDenom+"_"+args.trigger+'_'+sample+'_'+'_TriggerEfficiency'+args.version+'.'+args.extension
	print 'Processing.......', outputFileName

	DenomOnly = inFileSample.Get( args.trigger+'TriggerEfficiency/'+name+'Denom'+cut ) 
	DenomOnly.Rebin(rebin)
	Denom = DenomOnly.Clone()
	PassingOnly = inFileSample.Get( args.trigger+'TriggerEfficiency/'+name+'Passing'+cut ) 
	PassingOnly.Rebin(rebin)
	Passing = PassingOnly.Clone()
	print Denom, Passing
	Efficiency = TGraphAsymmErrors( Passing, Denom, 'cp'  )
	#Efficiency = TEfficiency( Passing, Denom )

	binWidth = DenomOnly.GetBinWidth(1)

	legend=TLegend(0.50,0.75,0.90,0.90)
	legend.SetFillStyle(0)
	legend.SetTextSize(0.04)

	DenomOnly.SetLineWidth(2)
	DenomOnly.SetLineColor(kRed-4)
	PassingOnly.SetLineWidth(2)
	PassingOnly.SetLineColor(kBlue-4)

	can = TCanvas('c1', 'c1',  10, 10, 750, 750 )
	pad1 = TPad("pad1", "Histo",0,0.46,1.00,1.00,-1)
	pad2 = TPad("pad2", "Efficiency",0,0.00,1.00,0.531,-1);
	pad1.Draw()
	pad2.Draw()

	pad1.cd()
	if log: pad1.SetLogy()

	legend.AddEntry( DenomOnly, triggerDenom+' (baseline trigger)', 'l' )
	legend.AddEntry( PassingOnly, args.trigger, 'l' )
	#DenomOnly.SetMinimum(10)
	DenomOnly.GetXaxis().SetRangeUser( xmin, xmax )
	DenomOnly.Draw('histe')
	DenomOnly.GetYaxis().SetTitleSize(0.06)
	DenomOnly.GetYaxis().SetTitleOffset(0.8)
	DenomOnly.GetYaxis().SetLabelSize(0.06)
	DenomOnly.GetXaxis().SetTitleOffset(0.8)
	DenomOnly.GetXaxis().SetTitleSize(0.06)
	DenomOnly.GetXaxis().SetLabelSize(0.05)
	PassingOnly.Draw('histe same')
	DenomOnly.GetYaxis().SetTitle( 'Events / '+str(binWidth) )

	CMS_lumi.CMS_lumi(pad1, 4, 0)
	legend.Draw()

	pad2.cd()
	pad2.SetTopMargin(0)
	pad2.SetBottomMargin(0.3)
	Efficiency.SetMarkerStyle(8)
	Efficiency.SetLineWidth(2)
	Efficiency.SetLineColor(kBlue-4)
	#Efficiency.SetFillStyle(1001)
	Efficiency.GetYaxis().SetTitle("Efficiency")
	Efficiency.GetYaxis().SetLabelSize(0.06)
	Efficiency.GetXaxis().SetLabelSize(0.06)
	Efficiency.GetYaxis().SetTitleSize(0.06)
	Efficiency.GetYaxis().SetTitleOffset(0.8)
	Efficiency.SetMinimum(-0.1)
	Efficiency.SetMaximum(1.1)
	Efficiency.GetXaxis().SetLimits( xmin, xmax )
	Efficiency.GetXaxis().SetTitle( xlabel )
	Efficiency.Draw()

	can.SaveAs( 'Plots/'+outputFileName.replace('.','Extended.') )
	del can

	#### Fitting
	#errF = TF1('errF', '0.5*(1+TMath::Erf((x-[0])/[1]))', 500, 1500 )
	#errF = TF1('errF', '0.5*(1+TMath::Erf(([0]*x-[1])/[2]))', 400, 1000 )  ## HT
	#errF = TF1('errF', '0.5*(1+TMath::Erf(([0]*x-[1])/[2]))', 0, 100 )  ## Mass
	#Efficiency.SetStatisticOption(TEfficiency.kFWilson)
	#for i in range(5): eff.Fit(errF, '+')
	#for i in range(5): Efficiency.Fit('errF', 'MIR')
	#print '&'*10, '900', errF.Eval(900)
	#print '&'*10, '1000', errF.Eval(1000)
	gStyle.SetOptFit(1)
	can1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
	Efficiency.SetMarkerStyle(8)
	Efficiency.SetMarkerColor(kGray)
	Efficiency.SetMinimum(-0.15)
	#Efficiency.SetMinimum(0.8)
	Efficiency.SetMaximum(1.15)
	Efficiency.GetXaxis().SetTitle( xlabel )
	Efficiency.GetYaxis().SetLabelSize(0.05)
	Efficiency.GetXaxis().SetLabelSize(0.05)
	Efficiency.GetYaxis().SetTitleSize(0.06)
	Efficiency.GetYaxis().SetTitleOffset(0.8)
	Efficiency.GetXaxis().SetTitleOffset(0.8)
	#Efficiency.GetXaxis().SetLimits( 400, 1200 )
	#Efficiency.GetXaxis().SetLimits( 700, 1050 )
	Efficiency.GetXaxis().SetLimits( xmin, xmax )
	Efficiency.Draw('AP')
	'''
	errF.SetLineColor(kRed)
	errF.SetLineWidth(2)
	errF.Draw('sames')
	can1.Update()
	st1 = Efficiency.GetListOfFunctions().FindObject("stats")
	st1.SetX1NDC(.60);
	st1.SetX2NDC(.90);
	st1.SetY1NDC(.20);
	st1.SetY2NDC(.50);
#	#eff.Draw("same")
	can1.Modified()
	'''
	
	'''
	rightmax = 1.2*PassingOnly.GetMaximum()
	rightmin = PassingOnly.GetMinimum()
	scale = gPad.GetUymax()/rightmax
	PassingOnly.SetLineColor(kBlue-5)
	PassingOnly.Scale( scale )
	PassingOnly.Draw( 'hist same' )
	#axis = TGaxis( gPad.GetUxmax(), gPad.GetUymin(), gPad.GetUxmax(), gPad.GetUymax(),-3,rightmax,710,"+L")
	axis = TGaxis( gPad.GetUxmax(), gPad.GetUymin(), gPad.GetUxmax(), gPad.GetUymax(),rightmin,rightmax,10,"+L")
	axis.SetTitle('Events / '+str(binWidth) )
	axis.SetTitleColor(kBlue-5)
	axis.SetTitleSize(0.06)
	axis.SetLabelSize(0.05)
	axis.SetTitleFont(42)
	axis.SetLabelFont(42)
	axis.SetLineColor(kBlue-5)
	axis.SetLabelColor(kBlue-5)
	axis.SetTitleOffset(0.7)
	axis.Draw()
	'''
	CMS_lumi.relPosX = 0.11
	CMS_lumi.cmsTextSize = 0.7
	CMS_lumi.extraOverCmsTextSize = 0.6
	CMS_lumi.CMS_lumi(can1, 4, 0)

	can1.SaveAs( 'Plots/'+outputFileName )
	del can1

	return Efficiency

def plotDiffEff( listOfEff, name ):
	"""docstring for plotDiffEff"""

	can = TCanvas('c1', 'c1',  10, 10, 750, 500 )

	legend=TLegend(0.60,0.25,0.90,0.40)
	legend.SetFillStyle(0)
	legend.SetTextSize(0.04)

	dummy = 1
	for sample in listOfEff: 
		legend.AddEntry( listOfEff[ sample ], sample, 'l' )

		listOfEff[ sample ].SetMarkerStyle(8)
		listOfEff[ sample ].SetLineWidth(2)
		listOfEff[ sample ].SetLineColor(dummy)
		listOfEff[ sample ].GetYaxis().SetTitle("Efficiency")
		listOfEff[ sample ].GetYaxis().SetLabelSize(0.06)
		listOfEff[ sample ].GetXaxis().SetLabelSize(0.06)
		listOfEff[ sample ].GetYaxis().SetTitleSize(0.06)
		listOfEff[ sample ].GetYaxis().SetTitleOffset(0.8)
		listOfEff[ sample ].SetMinimum(0.8)
		listOfEff[ sample ].SetMaximum(1.05)
		#listOfEff[ sample ].GetXaxis().SetLimits( 850, 950 )
		listOfEff[ sample ].GetXaxis().SetLimits( 0, 200 )
		if dummy == 1:
			labelAxis( name, listOfEff[ sample ], 'Pruned')
			listOfEff[ sample ].Draw()
		else: 
			listOfEff[ sample ].Draw('same')
		dummy+=1

	legend.Draw('same')
	CMS_lumi.lumi_13TeV = ""
	CMS_lumi.relPosX = 0.11
	CMS_lumi.cmsTextSize = 0.7
	CMS_lumi.extraOverCmsTextSize = 0.6
	CMS_lumi.CMS_lumi(can, 4, 0)
	can.SaveAs( 'Plots/'+name+'_DiffEfficiencies.'+args.extension )
	del can

###### Rebin 2D plots
def Rebin2D( h1, rebinx, rebiny ):
	"""docstring for Rebin2D"""

	tmph1 = h1.Clone()
	nbinsx = h1.GetXaxis().GetNbins()
	nbinsy = h1.GetYaxis().GetNbins()
	xmin  = h1.GetXaxis().GetXmin()
	xmax  = h1.GetXaxis().GetXmax()
	ymin  = h1.GetYaxis().GetXmin()
	ymax  = h1.GetYaxis().GetXmax()
	nx = nbinsx/rebinx
	ny = nbinsy/rebiny
	h1.SetBins( nx, xmin, xmax, ny, ymin, ymax )

	for biny in range( 1, nbinsy):
		for binx in range(1, nbinsx):
			ibin1 = h1.GetBin(binx,biny)
			h1.SetBinContent( ibin1, 0 )
		
	for biny in range( 1, nbinsy):
		by = tmph1.GetYaxis().GetBinCenter( biny )
		iy = h1.GetYaxis().FindBin(by)
		for binx in range(1, nbinsx):
			bx = tmph1.GetXaxis().GetBinCenter(binx)
			ix  = h1.GetXaxis().FindBin(bx)
			bin = tmph1.GetBin(binx,biny)
			ibin= h1.GetBin(ix,iy)
			cu  = tmph1.GetBinContent(bin)
			h1.AddBinContent(ibin,cu)
	return h1


def plot2DTriggerEfficiency( inFileSample, dataset, triggerDenom, name, xlabel, ylabel, Xmin, Xmax, rebinx, Ymin, Ymax, rebiny, labX, labY ):
	"""docstring for plot"""

	outputFileName = name+'_'+args.cut+'_'+triggerDenom+"_"+args.trigger+'_'+dataset+'_'+'TriggerEfficiency'+args.version+'.'+args.extension
	print 'Processing.......', outputFileName

	print args.trigger+'TriggerEfficiency/'+name+'Denom'+args.cut
	rawDenom = inFileSample.Get( args.trigger+'TriggerEfficiency/'+name+'Denom'+args.cut )
	Denom = Rebin2D( rawDenom, rebinx, rebiny )

	rawPassing = inFileSample.Get( args.trigger+'TriggerEfficiency/'+name+'Passing'+args.cut )
	Passing = Rebin2D( rawPassing, rebinx, rebiny )

	
	'''
	if ( TEfficiency.CheckConsistency( Passing, Denom ) ): Efficiency = TEfficiency( Passing, Denom )
	else: 
		print '--- Passing and Denom are inconsistent.'
		#sys.exit(0)
	'''

	Efficiency = Denom.Clone() 
	Efficiency.Reset()
	Efficiency.Divide( Passing, Denom, 1, 1, 'B' )

	
	tdrStyle.SetPadRightMargin(0.12)
	can = TCanvas('c1', 'c1',  10, 10, 1000, 750 )
	gStyle.SetPaintTextFormat("4.2f")
	Efficiency.SetMarkerSize(0.01)
	Efficiency.SetMaximum(1)
	Efficiency.SetMinimum(0)
	Efficiency.Draw('colz')
	Efficiency.Draw('same text')
	#gPad.Update()
	Efficiency.GetYaxis().SetTitleOffset(1.0)
	Efficiency.SetMarkerSize(2)
	Efficiency.GetXaxis().SetRange( int(Xmin/(rebinx)), int(Xmax/(rebinx)) )
	Efficiency.GetXaxis().SetTitle( xlabel )
	Efficiency.GetYaxis().SetTitle( ylabel )
	Efficiency.GetYaxis().SetRange( int(Ymin/(rebiny)), int(Ymax/(rebiny)) )
	#gPad.Update()

	CMS_lumi.relPosX = 0.13
	CMS_lumi.CMS_lumi(can, 4, 0)

	can.SaveAs( 'Plots/'+outputFileName )
	del can

#	##### 1D Efficiency
#	newEfficiency = TGraphAsymmErrors( newPassing, newDenom, 'cp'  )
#	binWidth = rebinx
#
#	#legend=TLegend(0.50,0.75,0.90,0.90)
#	#legend.SetFillStyle(0)
#	#legend.SetTextSize(0.04)
#
#	#gStyle.SetOptFit(1)
#	can1 = TCanvas('can1', 'can1',  10, 10, 750, 500 )
#	newEfficiency.SetMarkerStyle(8)
#	newEfficiency.SetMarkerColor(kGray)
#	newEfficiency.SetMinimum(-0.15)
#	#newEfficiency.SetMinimum(0.7)
#	newEfficiency.SetMaximum(1.15)
#	newEfficiency.GetYaxis().SetLabelSize(0.05)
#	newEfficiency.GetXaxis().SetLabelSize(0.05)
#	newEfficiency.GetYaxis().SetTitleSize(0.06)
#	newEfficiency.GetYaxis().SetTitleOffset(0.8)
#	newEfficiency.GetXaxis().SetTitleOffset(0.8)
#	#newEfficiency.GetXaxis().SetLimits( 400, 1200 )
#	#newEfficiency.GetXaxis().SetLimits( 700, 1050 )
#	newEfficiency.GetXaxis().SetLimits( 0, 200 ) #xmin, xmax )
#	newEfficiency.Draw('AP')
#	labelAxis( name, newEfficiency, 'Pruned')
#	CMS_lumi.relPosX = 0.11
#	CMS_lumi.cmsTextSize = 0.7
#	CMS_lumi.extraOverCmsTextSize = 0.6
#	CMS_lumi.CMS_lumi(can1, 4, 0)
#
#	outputFileName = outputFileName.replace( 'jet1Pt', ''  )
#	can1.SaveAs( 'Plots/'+outputFileName )
#	del can1
#
#	return Efficiency


def diffplotTriggerEfficiency( inFileSamples, name, cut, xmin, xmax, rebin, labX, labY, log, PU ):
	"""docstring for plot"""

	outputFileName = name+'_'+cut+"_"+args.trigger+'_'+'_TriggerEfficiency'+args.version+'.'+args.extension
	print 'Processing.......', outputFileName

	diffEffDenom = {}
	diffEffPassing = {}
	diffEff = {}
	for sam in inFileSamples:
		diffEffDenom[ sam ] = inFileSamples[ sam ].Get( 'TriggerEfficiency'+args.trigger+'/'+name+'Denom_'+cut ) #cutDijet' ) #+cut )
		diffEffDenom[ sam ].Rebin(rebin)
		diffEffPassing[ sam ] = inFileSamples[ sam ].Get( 'TriggerEfficiency'+args.trigger+'/'+name+'Passing_'+cut ) #cutHT' ) #+cut )
		diffEffPassing[ sam ].Rebin(rebin)
		diffEff[ sam ] = TGraphAsymmErrors( diffEffPassing[sam], diffEffDenom[sam], 'cp'  )
		#Efficiency = TEfficiency( Passing, Denom )

	#binWidth = DenomOnly.GetBinWidth(1)

	legend=TLegend(0.50,0.75,0.90,0.90)
	legend.SetFillStyle(0)
	legend.SetTextSize(0.04)

	can1 = TCanvas('c1', 'c1',  10, 10, 750, 500 )
	dummy=1
	for q in diffEff:
		diffEff[q].SetMarkerStyle(8)
		diffEff[q].SetMarkerColor(dummy)
		legend.AddEntry( diffEff[ q ], q, 'pl' )
		dummy+=1

	diffEff[diffEff.iterkeys().next()].SetMinimum(0.8)
	diffEff[diffEff.iterkeys().next()].SetMaximum(1.15)
	diffEff[diffEff.iterkeys().next()].GetYaxis().SetLabelSize(0.05)
	diffEff[diffEff.iterkeys().next()].GetXaxis().SetLabelSize(0.05)
	diffEff[diffEff.iterkeys().next()].GetYaxis().SetTitleSize(0.06)
	diffEff[diffEff.iterkeys().next()].GetYaxis().SetTitleOffset(0.8)
	diffEff[diffEff.iterkeys().next()].GetXaxis().SetTitleOffset(0.8)
	diffEff[diffEff.iterkeys().next()].GetXaxis().SetLimits( xmin, xmax )
	diffEff[diffEff.iterkeys().next()].Draw('AP')
	for q in diffEff: diffEff[q].Draw("P same")
	labelAxis( name, diffEff[diffEff.iterkeys().next()], 'SoftDrop')
	CMS_lumi.relPosX = 0.11
	CMS_lumi.cmsTextSize = 0.7
	CMS_lumi.extraOverCmsTextSize = 0.6
	CMS_lumi.CMS_lumi(can1, 4, 0)
	legend.Draw()

	can1.SaveAs( 'Plots/'+outputFileName )
	del can1

def plot2D( inFile, sample, name, titleXAxis, titleXAxis2, Xmin, Xmax, rebinx, Ymin, Ymax, rebiny, legX, legY ):
	"""docstring for plot"""

	outputFileName = name+args.cut+'_'+args.trigger+'_'+sample+'_'+'TriggerEfficiency'+args.version+'.'+args.extension
	print 'Processing.......', outputFileName
	h1 = inFile.Get( args.trigger+'TriggerEfficiency/'+name+args.cut )
	tmph1 = h1.Clone()
	
	### Rebinning
	nbinsx = h1.GetXaxis().GetNbins()
	nbinsy = h1.GetYaxis().GetNbins()
	xmin  = h1.GetXaxis().GetXmin()
	xmax  = h1.GetXaxis().GetXmax()
	ymin  = h1.GetYaxis().GetXmin()
	ymax  = h1.GetYaxis().GetXmax()
	nx = nbinsx/rebinx
	ny = nbinsy/rebiny
	h1.SetBins( nx, xmin, xmax, ny, ymin, ymax )

	for biny in range( 1, nbinsy):
		for binx in range(1, nbinsx):
			ibin1 = h1.GetBin(binx,biny)
			h1.SetBinContent( ibin1, 0 )
		
	for biny in range( 1, nbinsy):
		by = tmph1.GetYaxis().GetBinCenter( biny )
		iy = h1.GetYaxis().FindBin(by)
		for binx in range(1, nbinsx):
			bx = tmph1.GetXaxis().GetBinCenter(binx)
			ix  = h1.GetXaxis().FindBin(bx)
			bin = tmph1.GetBin(binx,biny)
			ibin= h1.GetBin(ix,iy)
			cu  = tmph1.GetBinContent(bin)
			h1.AddBinContent(ibin,cu)

	h1.GetXaxis().SetTitle( titleXAxis )
	h1.GetYaxis().SetTitleOffset( 1.0 )
	h1.GetYaxis().SetTitle( titleXAxis2 )

	if (Xmax or Ymax):
		h1.GetXaxis().SetRangeUser( Xmin, Xmax )
		h1.GetYaxis().SetRangeUser( Ymin, Ymax )

	tdrStyle.SetPadRightMargin(0.12)
	can = TCanvas('c1', 'c1',  750, 500 )
	can.SetLogz()
	#h1.SetMaximum(100)
	#h1.SetMinimum(0.1)
	h1.Draw('colz')

	CMS_lumi.extraText = "Simulation Preliminary"
	CMS_lumi.relPosX = 0.13
	CMS_lumi.CMS_lumi(can, 4, 0)

	can.SaveAs( 'Plots/'+outputFileName )
	del can



if __name__ == '__main__':

	parser = argparse.ArgumentParser()
	parser.add_argument('-p', '--proc', action='store', default='1D', help='Process to draw, example: 1D, 2D, MC.' )
	parser.add_argument('-d', '--dataset', action='store', default='JetHT', help='Dataset: JetHT, SingleMuon, etc.' )
	parser.add_argument('-v', '--version', action='store', default='v01', help='Version of the files' )
	parser.add_argument('-C', '--cut', action='store', default='_cutDEta', help='cut, example: cutDEta' )
	parser.add_argument('-s', '--single', action='store', default='all', help='single histogram, example: massAve_cutDijet.' )
	parser.add_argument('-l', '--lumi', action='store', default='15.5', help='Luminosity, example: 1.' )
	parser.add_argument('-t', '--trigger', action='store', default='AK8PFHT700TrimMass50', help="Trigger used, name of directory" )
	parser.add_argument('-e', '--extension', action='store', default='png', help='Extension of plots.' )

	try:
		args = parser.parse_args()
	except:
		parser.print_help()
		sys.exit(0)

	triggerlabX = 0.15
	triggerlabY = 1.0
	jetMassHTlabX = 0.87
	jetMassHTlabY = 0.20

	HTMinX = 300
	HTMaxX = 1500

	plotList = [ 

		[ '1D', 'HT', 500, 2000, 'HT [GeV]', 20, triggerlabX, triggerlabY, True],
		#[ '1D', 'HT', 800, 1200, 1, triggerlabX, triggerlabY, True],
		[ '1D', 'jet1Pt', 300, 1000, 'Leading Jet Pt [GeV]',  10, triggerlabX, triggerlabY, True],
#		[ '1D', 'jet2Pt', ptMinX, ptMaxX, 2, triggerlabX, triggerlabY, True],
		[ '1D', 'jet1SoftDropMass', 0, 500, 'Leading Jet Softdrop Mass [GeV]', 10, triggerlabX, triggerlabY, True],
		[ '1D', 'massAve', 0, 500, 10, triggerlabX, triggerlabY, True],
		[ 'tmp', 'jet1SoftDropMass', 0, 500, 1, triggerlabX, triggerlabY, True],

		#[ '2D', 'jetMassHTDenom_noTrigger', 'Leading Trimmed Jet Mass [GeV]', 'H_{T} [GeV]', 0, 200, 2, 100, HTMaxX, 5, 0.85, 0.2],
		#[ '2D', 'jetTrimmedMassHTDenom_noTrigger', 'Leading Trimmed Jet Mass [GeV]', 'H_{T} [GeV]', 0, 200, 2, 100, HTMaxX, 5, 0.85, 0.2],
		#[ '2D', 'jetMassHTDenom_triggerOne', 'Leading Trimmed Jet Mass [GeV]', 'H_{T} [GeV]', 0, 200, 2, 100, HTMaxX, 5, 0.85, 0.2],
		#[ '2D', 'jetMassHTDenom_triggerTwo', 'Leading Trimmed Jet Mass [GeV]', 'H_{T} [GeV]', 0, 200, 2, 100, HTMaxX, 5, 0.85, 0.2],
		#[ '2D', 'jetMassHTDenom_triggerOneAndTwo', 'Leading Trimmed Jet Mass [GeV]', 'H_{T} [GeV]', 0, 200, 2, 100, HTMaxX, 5, 0.85, 0.25],
		[ '2d', 'jet1SDMassvsHT', 'Leading SD Jet Mass [GeV]', 'H_{T} [GeV]', 0, 200, 20, 900, 1500, 50, 0.85, 0.2],
		[ '2d', 'jet1SDMassvsPt', 'Leading SD Jet Mass [GeV]', 'Leading Jet Pt [GeV]', 0, 200, 20, 400, 800, 50, 0.85, 0.2],


		[ '2D', 'prunedMassAveHT', 'Leading Jet Pruned Mass [GeV]', 'HT [GeV]', 20, 200, 2, HTMinX, 1200, 10, jetMassHTlabX, jetMassHTlabY],
		#[ '2D', 'jetMassHT', 20, 200, 2, HTMinX, 1200, 10, jetMassHTlabX, jetMassHTlabY],
		[ '2D', 'jet4PtHT', '4th jet Pt [GeV]', 'HT [GeV]',  60, 300, 2, HTMinX, 1200, 10, jetMassHTlabX, jetMassHTlabY],
		[ '2D', 'jet1SDMassvsHT', 'Leading SD Jet Mass [GeV]', 'H_{T} [GeV]', 20, 200, 20, 900, 1500, 50, 0.85, 0.2],
		[ '2D', 'jet1SDMassvsPt', 'Leading SD Jet Mass [GeV]', 'Leading Jet Pt [GeV]', 20, 200, 20, 400, 800, 50, 0.85, 0.2],
		]

	if 'all' in args.single: Plots = [ x[1:] for x in plotList if x[0] in args.proc ]
	else: Plots = [ y[1:] for y in plotList if ( ( y[0] in args.proc ) and ( y[1] in args.single ) )  ]


	effList = {}
	bkgFiles = {}
	signalFiles = {}
	CMS_lumi.extraText = "Preliminary"

	Samples = {}

	Samples[ 'JetHT2017' ] = [ 'TriggerValAndEff_JetHT-Run2017A.root', 0 ] 
	Samples[ 'RPV80' ] = [ 'RPVStopStopToJets_UDD312_M-80_TriggerEfficiencies_v04.root', 0 ] 
	Samples[ 'TTJets' ] = [ 'TT_TriggerEfficiencies_v03.root', 0 ] 
	Samples[ 'QCD' ] = [ 'QCD_Pt-15to3000_TriggerEfficiencies_v03.root', 0 ] 
	#Samples[ 'SingleMuon2017A' ] = [ 'TriggerValAndEff_SingleMuon-Run2017A.root', 0 ] 
	#Samples[ 'SingleMuon2017B' ] = [ 'TriggerEfficiencies_SingleMuon_Run2017B_v01.root', 4511 ] 
	#Samples[ 'SingleMuon2017C' ] = [ 'TriggerEfficiencies_SingleMuon_Run2017C-PromptReco-v1_v01.root', 1284 ] 
	Samples[ 'SingleMuon2017C' ] = [ 'TriggerEfficiencies_SingleMuon_Run2017C-PromptReco_v02.root', 2413 ] 
	#Samples[ 'SingleMuon2017All' ] = [ 'TriggerEfficiencies_SingleMuon_Run2017All_v01.root', 5795 ] 


	processingSamples = {}
	if 'all' in args.dataset: 
		for sam in Samples: processingSamples[ sam ] = Samples[ sam ]
	else:
		for sam in Samples: 
			if sam.startswith( args.dataset ): processingSamples[ sam ] = Samples[ sam ]

	if len(processingSamples)==0: print 'No sample found. \n Have a nice day :)'

	for sam in processingSamples:

		CMS_lumi.lumi_13TeV = '' #str( round( (processingSamples[sam][1]/1000.), 1 ) )+" fb^{-1}"
		if 'SingleMu' in sam: BASEDTrigger = 'IsoMu27'
		elif 'JetHT' in sam: BASEDTrigger = 'PFJet40'
		else: BASEDTrigger = ''

		for i in Plots:
			if '1D' in args.proc:
				effList[ sam ] = plotTriggerEfficiency( TFile.Open('Rootfiles/'+processingSamples[sam][0]), 
									sam, 
									BASEDTrigger, 
									i[0], args.cut, i[1], i[2], i[3], i[4], i[5], i[6], i[7] )
			elif '2D' in args.proc:
				plot2DTriggerEfficiency( TFile.Open('Rootfiles/'+processingSamples[sam][0]), 
							sam, 
							BASEDTrigger, 
							i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8], i[9], i[10] )

			elif '2d' in args.proc: 
				plot2D( TFile.Open('Rootfiles/'+processingSamples[sam][0]),
						sam,
						i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8], i[9], i[10] )
