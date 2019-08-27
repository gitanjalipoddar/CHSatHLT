#!/usr/bin/env python
# coding: utf-8

# In[16]:


import matplotlib
matplotlib.use('Agg')
import ROOT as root
from rootpy.io import root_open
import matplotlib.pyplot as plt
import rootpy.plotting.root2matplotlib as rplt
import numpy as np


# In[17]:


f_offline=root_open("DQM_V0001_R000000001__Global__CMSSW_X_Y_Z__RECO_ofline.root","read")
f_online=root_open("DQM_V0001_R000000001__Global__CMSSW_X_Y_Z__RECO_online.root","read")


# In[90]:


#plotting function
def plotting(hist1,hist2,i):
    #cloning original histograms
    h1=hist1.Clone(title='Full')
    h2=hist2.Clone(title='Partial')

    #normalising
    N1=h1.Integral()
    N2=h2.Integral()
    if N1==0:
        print 'Integral of histogram '+i+' equals zero. Cannot normalise'
    else:
        h1.Scale(1/N1)
    if N2==0:
        print 'Integral of histogram '+i+' equals zero. Cannot normalise'
    else:
        h2.Scale(1/N2)

    #dividing histograms for ratio
    h3=h1.Clone()
    h3.linecolor='black'
    h3.linewidth=1
    h3.Divide(h2)

    #plotting
    fig=plt.figure(1)
    plt.subplot2grid((5,3),(0,0), rowspan=4,colspan=4)
    h1.linewidth = 1
    h1.linecolor = 'red'
    h2.linewidth =1
    h2.linecolor= 'blue'
    rplt.errorbar(h1)
    rplt.errorbar(h2)
    plt.gca().yaxis.grid(True)
    plt.legend(loc='best')

    #setting limits
    if i=='ptSIM':
        plt.xlim(right=200)
    if i=='tracksSIM':
        plt.xlim(right=2000)
    if i=='tracks':
        plt.xlim(right=350)
    if i=='fakes':
        plt.xlim(right=60)
    if i=='vertposSIM':
        plt.xlim(left=-0.5,right=0.5)
    if i=='bunchxSIM':
        plt.xlim(left=-1,right=1)
    if i=='effic_vs_3Dlayer':
        plt.xlim(right=17)
    if i=="effic_vs_pixellayer":
        plt.xlim(right=8)
    if i=="effic_vs_pu":
        plt.xlim(right=2)
    if i=='fakerate_vs_3Dlayer':
        plt.xlim(right=17)
    if i=='fakerate_vs_hit':
        plt.xlim(right=35)
    if i=="fakerate_vs_pixellayer":
        plt.xlim(right=8)
    if i=="fakerate_vs_pu":
        plt.xlim(right=2)
    if i=='duplicatesRate_3Dlayer':
        plt.xlim(right=17)
    if i=='duplicatesRate_dr':
        plt.xlim(right=0.15)
    if i=="duplicatesRate_hit":
        plt.xlim(right=50)
    if i=="duplicatesRate_pixellayer":
        plt.xlim(right=8)
    if i=="duplicatesRate_pu":
        plt.xlim(right=2)
    if i=='duplicatesRate_Pt':
        plt.xlim(right=400)
    if i=='chargeMisIdRate_3Dlayer':
        plt.xlim(right=17)
    if i=="chargeMisIdRate_pixellayer":
        plt.xlim(right=8)
    if i=="chargeMisIdRate_pu":
        plt.xlim(right=2)
    if i=="chargeMisIdRate_hit":
        plt.xlim(right=25)
    if i=='hits':
        plt.xlim(right=50)
    if i=='losthits':
        plt.xlim(right=10)
    if i=='eta':
        plt.xlim(left=-0.025,right=0.025)
    if i=='pullDxy':
        plt.xlim(left=-5,right=5)
    if i=='pullDz':
        plt.xlim(left=-5,right=5)
    if i=='pullPhi':
        plt.xlim(left=-5,right=5)
    if i=='pullQoverp':
        plt.xlim(left=-5,right=5)
    if i=='pullTheta':
        plt.xlim(left=-5,right=5)
    if i=="cotThetares_vs_pt_Sigma":
        plt.xlim(right=700)
    if i=="dxyres_vs_pt_Sigma":
        plt.xlim(right=700)
    if i=="dzres_vs_pt_Sigma":
        plt.xlim(right=700)
    if i=="phires_vs_pt_Sigma":
        plt.xlim(right=700)
    if i=="ptres_vs_pt_Sigma":
        plt.xlim(right=700)

    #ratio plot
    plt.subplot2grid((5,3),(4,0),rowspan=2,colspan=4)
    rplt.errorbar(h3)
    plt.ylim(0.8,1.2)
    plt.ylabel('Ratio')
    plt.gca().yaxis.grid(True)

    #setting limits
    if i=='ptSIM':
        plt.xlim(right=200)
    if i=='tracksSIM':
        plt.xlim(right=2000)
    if i=='tracks':
        plt.xlim(right=350)
    if i=='fakes':
        plt.xlim(right=60)
    if i=='vertposSIM':
        plt.xlim(left=-0.5,right=0.5)
    if i=='bunchxSIM':
        plt.xlim(left=-1,right=1)
    if i=='effic_vs_3Dlayer':
        plt.xlim(right=17)
    if i=="effic_vs_pixellayer":
        plt.xlim(right=8)
    if i=="effic_vs_pu":
        plt.xlim(right=2)
    if i=='fakerate_vs_3Dlayer':
        plt.xlim(right=17)
    if i=='fakerate_vs_hit':
        plt.xlim(right=35)
    if i=="fakerate_vs_pixellayer":
        plt.xlim(right=8)
    if i=="fakerate_vs_pu":
        plt.xlim(right=2)
    if i=='duplicatesRate_3Dlayer':
        plt.xlim(right=17)
    if i=='duplicatesRate_dr':
        plt.xlim(right=0.15)
    if i=="duplicatesRate_hit":
        plt.xlim(right=50)
    if i=="duplicatesRate_pixellayer":
        plt.xlim(right=8)
    if i=="duplicatesRate_pu":
        plt.xlim(right=2)
    if i=='duplicatesRate_Pt':
        plt.xlim(right=400)
    if i=='chargeMisIdRate_3Dlayer':
        plt.xlim(right=17)
    if i=="chargeMisIdRate_pixellayer":
        plt.xlim(right=8)
    if i=="chargeMisIdRate_pu":
        plt.xlim(right=2)
    if i=="chargeMisIdRate_hit":
        plt.xlim(right=25)
    if i=='hits':
        plt.xlim(right=50)
    if i=='losthits':
        plt.xlim(right=10)
    if i=='eta':
        plt.xlim(left=-0.025,right=0.025)
    if i=='pullDxy':
        plt.xlim(left=-5,right=5)
    if i=='pullDz':
        plt.xlim(left=-5,right=5)
    if i=='pullPhi':
        plt.xlim(left=-5,right=5)
    if i=='pullQoverp':
        plt.xlim(left=-5,right=5)
    if i=='pullTheta':
        plt.xlim(left=-5,right=5)
    if i=="cotThetares_vs_pt_Sigma":
        plt.xlim(right=700)
    if i=="dxyres_vs_pt_Sigma":
        plt.xlim(right=700)
    if i=="dzres_vs_pt_Sigma":
        plt.xlim(right=700)
    if i=="phires_vs_pt_Sigma":
        plt.xlim(right=700)
    if i=="ptres_vs_pt_Sigma":
        plt.xlim(right=700)

    fig.suptitle(i)

    fig.set_size_inches(7, 9)
    plt.savefig(i+".png")
    plt.close()  


# In[87]:


#2d plots
def plotting2(hist1,hist2,i):
    #cloning original histograms
    h1=hist1.Clone(title='Full')
    h2=hist2.Clone(title='Partial')
    
    fig=plt.figure(1)
    rplt.hist2d(h1)
    fig.suptitle(i+"_full")
    plt.savefig(i+"_full.png")
    plt.close() 

    fig=plt.figure(1)
    rplt.hist2d(h2)
    fig.suptitle(i+"_partial")
    plt.savefig(i+"_partial.png")
    plt.close() 


# In[61]:


#plots for cross checking with simulation
arr=["ptSIM","etaSIM","vertposSIM","tracksSIM","bunchxSIM"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/simulation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/simulation/"+i)
    
    plotting(hist1,hist2,i) 


# In[4]:


#validation summary plots
arr=["duplicatesRate_coll","effic_vs_coll","fakerate_vs_coll","num_assoc(recoToSim)_coll",
            "num_assoc(simToReco)_coll","num_duplicate_coll","num_reco_coll","num_simul_coll"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/"+i)

    plotting(hist1,hist2,i)


# In[7]:


#validation plots per input track collection

#global tracking performances
arr=["tracks","fakes","effic"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting(hist1,hist2,i)


# In[15]:


#validation plots per input track collection

#global tracking performances
arr=["efficPt","effic_vs_3Dlayer","effic_vs_dr","effic_vs_dxy",
     "effic_vs_dz","effic_vs_hit","effic_vs_layer","effic_vs_phi","effic_vs_pixellayer",
     "effic_vs_pu","effic_vs_vertpos","effic_vs_zpos"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting(hist1,hist2,i)


# In[29]:


#validation plots per input track collection

#global tracking performances
arr=["fakerate","fakeratePt","fakerate_vs_3Dlayer","fakerate_vs_chi2","fakerate_vs_dr",
     "fakerate_vs_dxy","fakerate_vs_dz","fakerate_vs_hit","fakerate_vs_layer","fakerate_vs_phi",
     "fakerate_vs_pixellayer","fakerate_vs_pu"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting(hist1,hist2,i)


# In[46]:


#validation plots per input track collection

#global tracking performances
arr=["duplicatesRate","duplicatesRate_Pt","duplicatesRate_3Dlayer","duplicatesRate_chi2",
     "duplicatesRate_dr","duplicatesRate_dxy","duplicatesRate_dz","duplicatesRate_hit",
     "duplicatesRate_layer","duplicatesRate_phi","duplicatesRate_pixellayer",
     "duplicatesRate_pu"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting(hist1,hist2,i)


# In[51]:


#validation plots per input track collection

#global tracking performances
arr=["chargeMisIdRate","chargeMisIdRate_Pt","chargeMisIdRate_3Dlayer","chargeMisIdRate_chi2",
     "chargeMisIdRate_dxy","chargeMisIdRate_dz","chargeMisIdRate_hit",
     "chargeMisIdRate_layer","chargeMisIdRate_phi","chargeMisIdRate_pixellayer",
     "chargeMisIdRate_pu"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting(hist1,hist2,i)


# In[53]:


#validation plots per input track collection

#global tracking performances
arr=["num_reco_eta","num_assoc(simToReco)_eta","num_assoc(recoToSim)_eta","num_simul_eta",
     "num_reco_pT","num_assoc(simToReco)_pT","num_assoc(recoToSim)_pT","num_simul_pT"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting(hist1,hist2,i)


# In[86]:


#validation 2d plots per input track collection

#global tracking performances

hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/nrec_vs_nsim")
hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/nrec_vs_nsim")

i="nrec_vs_nsim"
plotting2(hist1,hist2,i)


# In[62]:


#number of hits, chi2 and charge distribution
arr=["chi2","chi2_prob","chi2mean"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting(hist1,hist2,i)


# In[68]:


#number of hits, chi2 and charge distribution
arr=["hits","losthits","hits_eta","losthits_vs_eta","charge"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting(hist1,hist2,i)


# In[79]:


#pulls and residues of track parameters
arr=["eta","pullPt","pullTheta","pullPhi","pullDxy","pullDz","pullQoverp"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting(hist1,hist2,i)


# In[80]:


#pulls and residues of track parameters
arr=["h_dxypulleta_Sigma","h_ptpulleta_Sigma","h_dzpulleta_Sigma","h_phipulleta_Sigma",
    "h_thetapulleta_Sigma"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting(hist1,hist2,i)


# In[88]:


#pulls and residues of track parameters
arr=["dxypull_vs_eta","ptpull_vs_eta","dzpull_vs_eta","phipull_vs_eta",
    "thetapull_vs_eta"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting2(hist1,hist2,i)


# In[91]:


#resolution track parameters
arr=["ptres_vs_eta_Sigma","ptres_vs_pt_Sigma","cotThetares_vs_eta_Sigma",
     "cotThetares_vs_pt_Sigma","phires_vs_eta_Sigma","phires_vs_pt_Sigma",
     "dxyres_vs_eta_Sigma","dxyres_vs_pt_Sigma","dzres_vs_eta_Sigma",
     "dzres_vs_pt_Sigma"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting(hist1,hist2,i)


# In[93]:


#resolution track parameters
arr=["etares_vs_eta","ptres_vs_eta","ptres_vs_pt","cotThetares_vs_eta","cotThetares_vs_pt",
     "phires_vs_eta","phires_vs_pt","dxyres_vs_eta","dxyres_vs_pt","dzres_vs_eta",
     "dzres_vs_pt"]

for i in arr:
    hist1=f_offline.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)
    hist2=f_online.Get("DQMData/Run 1/Tracking/Run summary/Track/general_trackingParticleRecoAsssociation/"+i)

    plotting2(hist1,hist2,i)


# In[ ]:




