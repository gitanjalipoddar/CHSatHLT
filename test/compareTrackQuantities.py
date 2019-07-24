import DataFormats.FWLite as fwlite 
import ROOT
import matplotlib.pyplot as plt
import numpy as np

events= fwlite.Events("output_offlineTracking.root")
handle=fwlite.Handle("std::vector<reco::Track>")
label="generalTracks"

fullChargePt=[]
fullPhi=[]
fullEta=[]
fullDxy=[]
fullDz=[]
for i in events:
    i.getByLabel(label,handle)
    tracks=handle.product()
    for j in tracks:
        fullChargePt.append(j.charge()/j.pt())
        fullPhi.append(j.phi())
        fullEta.append(j.eta())
        fullDxy.append(j.dxy())
        fullDz.append(j.dz())

events= fwlite.Events("output_fullTracking.root")
handle=fwlite.Handle("std::vector<reco::Track>")
label="generalTracks"

partialChargePt=[]
partialPhi=[]
partialEta=[]
partialDxy=[]
partialDz=[]
for i in events:
    i.getByLabel(label,handle)
    tracks=handle.product()
    for j in tracks:
        partialChargePt.append(j.charge()/j.pt())
        partialPhi.append(j.phi())
        partialEta.append(j.eta())
        partialDxy.append(j.dxy())
        partialDz.append(j.dz())

print "Number of tracks for 1000 events in full tracking: ", len(fullChargePt)
print "Number of tracks for 1000 events in partial tracking: ", len(partialChargePt)


#plotting charge/pt
fig=plt.figure(1)
plt.subplot2grid((5,3),(0,0), rowspan=4,colspan=4)

bins=np.arange(-10,11,step=2)
x=np.arange(-9,10,step=2)
full=np.histogram(fullChargePt,bins=bins,normed=True) 
partial=np.histogram(partialChargePt,bins=bins,normed=True)


a=np.histogram(fullChargePt,bins=bins)
err_a=np.sqrt(a[0])
full_e=err_a/a[0]*full[0]

a=np.histogram(partialChargePt,bins=bins)
err_a=np.sqrt(a[0])
partial_e=err_a/a[0]*partial[0]

err_x=np.ones_like(partial[0])*1

plt.plot(x,partial[0],'ro',markersize=4, label='Partial')
plt.errorbar(x,partial[0],yerr=partial_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.plot(x,full[0],'bo',markersize=4,label='Full')
plt.errorbar(x,full[0],yerr=full_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.gca().yaxis.grid(True)
#plt.xticks(x)
plt.legend()
plt.ylabel("Normalised tracks/2")

#plotting ratio plot
plt.subplot2grid((5,3),(4,0),rowspan=2,colspan=4)
ratio=partial[0]/full[0]
ratio_e=np.abs(ratio)*np.sqrt(np.square(np.divide(partial_e,partial[0]))+np.square(np.divide(full_e,full[0])))

plt.plot(x,ratio,'bo',markersize=2)
plt.ylim(0.8,1.2)
plt.errorbar(x,ratio,yerr=ratio_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.gca().yaxis.grid(True)
plt.ylabel('Partial/Full')
#plt.xticks(x)

plt.xlabel("q/p$_{T}$")
fig.set_size_inches(7, 9)
plt.savefig("track_chargept.png")
plt.close()

#plotting phi
fig=plt.figure(1)
plt.subplot2grid((5,3),(0,0), rowspan=4,colspan=4)

bins=np.arange(-3,3.5,step=0.5)
x=np.arange(-2.75,3,step=0.5)

full=np.histogram(fullPhi,bins=bins,normed=True) 
partial=np.histogram(partialPhi,bins=bins,normed=True)

a=np.histogram(fullPhi,bins=bins)
err_a=np.sqrt(a[0])
full_e=err_a/a[0]*full[0]

a=np.histogram(partialPhi,bins=bins)
err_a=np.sqrt(a[0])
partial_e=err_a/a[0]*partial[0]

err_x=np.ones_like(partial[0])*0.25

plt.plot(x,partial[0],'ro',markersize=4, label='Partial')
plt.errorbar(x,partial[0],yerr=partial_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.plot(x,full[0],'bo',markersize=4,label='Full')
plt.errorbar(x,full[0],yerr=full_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.gca().yaxis.grid(True)
plt.ylim(0,0.2)
plt.legend()
plt.ylabel("Normalised tracks/0.5")

#plotting ratio plot
plt.subplot2grid((5,3),(4,0),rowspan=2,colspan=4)
ratio=partial[0]/full[0]
ratio_e=np.abs(ratio)*np.sqrt(np.square(np.divide(partial_e,partial[0]))+np.square(np.divide(full_e,full[0])))

plt.plot(x,ratio,'bo',markersize=2)
plt.errorbar(x,ratio,yerr=ratio_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.ylim(0.8,1.2)
plt.gca().yaxis.grid(True)
plt.ylabel('Partial/Full')
#plt.xticks(x)

plt.xlabel("$\phi$")
fig.set_size_inches(7, 9)
plt.savefig("track_phi.png")
plt.close()

#plotting eta
fig=plt.figure(1)
plt.subplot2grid((5,3),(0,0), rowspan=4,colspan=4)

bins=np.arange(-3,3.5,step=0.5)
x=np.arange(-2.75,3,step=0.5)

full=np.histogram(fullEta,bins=bins,normed=True) 
partial=np.histogram(partialEta,bins=bins,normed=True)

a=np.histogram(fullEta,bins=bins)
err_a=np.sqrt(a[0])
full_e=err_a/a[0]*full[0]

a=np.histogram(partialEta,bins=bins)
err_a=np.sqrt(a[0])
partial_e=err_a/a[0]*partial[0]

err_x=np.ones_like(partial[0])*0.25

plt.plot(x,partial[0],'ro',markersize=4, label='Partial')
plt.errorbar(x,partial[0],yerr=partial_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.plot(x,full[0],'bo',markersize=4,label='Full')
plt.errorbar(x,full[0],yerr=full_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.gca().yaxis.grid(True)
plt.ylim(0,0.25)
plt.legend()
plt.ylabel("Normalised tracks/0.5")

#plotting ratio plot
plt.subplot2grid((5,3),(4,0),rowspan=2,colspan=4)
ratio=partial[0]/full[0]
ratio_e=np.abs(ratio)*np.sqrt(np.square(np.divide(partial_e,partial[0]))+np.square(np.divide(full_e,full[0])))

plt.plot(x,ratio,'bo',markersize=2)
plt.errorbar(x,ratio,yerr=ratio_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.ylim(0.8,1.2)
plt.gca().yaxis.grid(True)
plt.ylabel('Partial/Full')
#plt.xticks(x)

plt.xlabel("$\eta$")
fig.set_size_inches(7, 9)
plt.savefig("track_eta.png")
plt.close()


#plotting dxy
fig=plt.figure(1)
plt.subplot2grid((5,3),(0,0), rowspan=4,colspan=4)

bins=np.arange(-0.5,0.6,step=0.125)
x=np.arange(-0.4375,0.45,step=0.125)
full=np.histogram(fullDxy,bins=bins,normed=True) 
partial=np.histogram(partialDxy,bins=bins,normed=True)

a=np.histogram(fullDxy,bins=bins)
err_a=np.sqrt(a[0])
full_e=err_a/a[0]*full[0]

a=np.histogram(partialDxy,bins=bins)
err_a=np.sqrt(a[0])
partial_e=err_a/a[0]*partial[0]

err_x=np.ones_like(partial[0])*0.0625

plt.plot(x,partial[0],'ro',markersize=4, label='Partial')
plt.errorbar(x,partial[0],yerr=partial_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.plot(x,full[0],'bo',markersize=4,label='Full')
plt.errorbar(x,full[0],yerr=full_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.gca().yaxis.grid(True)
plt.legend()
plt.ylabel("Normalised tracks/0.125")

#plotting ratio plot
plt.subplot2grid((5,3),(4,0),rowspan=2,colspan=4)
ratio=partial[0]/full[0]
ratio_e=np.abs(ratio)*np.sqrt(np.square(np.divide(partial_e,partial[0]))+np.square(np.divide(full_e,full[0])))

plt.plot(x,ratio,'bo',markersize=2)
plt.errorbar(x,ratio,yerr=ratio_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.ylim(0.8,1.2)
plt.gca().yaxis.grid(True)
plt.ylabel('Partial/Full')

plt.xlabel("d$_{xy}$")
fig.set_size_inches(7, 9)
plt.savefig("track_dxy.png")
plt.close()

#plotting dz
fig=plt.figure(1)
plt.subplot2grid((5,3),(0,0), rowspan=4,colspan=4)

bins=np.arange(-10,10.5,step=1)
x=np.arange(-9.5,9.6,step=1)
full=np.histogram(fullDz,bins=bins,normed=True) 
partial=np.histogram(partialDz,bins=bins,normed=True)

a=np.histogram(fullDz,bins=bins)
err_a=np.sqrt(a[0])
full_e=err_a/a[0]*full[0]

a=np.histogram(partialDz,bins=bins)
err_a=np.sqrt(a[0])
partial_e=err_a/a[0]*partial[0]

err_x=np.ones_like(partial[0])*0.5

plt.plot(x,partial[0],'ro',markersize=4, label='Partial')
plt.errorbar(x,partial[0],yerr=partial_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.plot(x,full[0],'bo',markersize=4,label='Full')
plt.errorbar(x,full[0],yerr=full_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.gca().yaxis.grid(True)
plt.legend()
#plt.xticks(x)
plt.ylabel("Normalised tracks/1")

#plotting ratio plot
plt.subplot2grid((5,3),(4,0),rowspan=2,colspan=4)
ratio=partial[0]/full[0]
ratio_e=np.abs(ratio)*np.sqrt(np.square(np.divide(partial_e,partial[0]))+np.square(np.divide(full_e,full[0])))

plt.plot(x,ratio,'bo',markersize=2)
plt.errorbar(x,ratio,yerr=ratio_e,xerr=err_x,ecolor='black',elinewidth='1',fmt='none')
plt.ylim(0.8,1.2)
plt.gca().yaxis.grid(True)
plt.ylabel('Partial/Full')

#plt.xticks(x)
plt.xlabel("d$_{z}$")
fig.set_size_inches(7, 9)
plt.savefig("track_dz.png")
plt.close()
