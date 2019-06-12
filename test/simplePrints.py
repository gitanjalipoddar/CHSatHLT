import DataFormats.FWLite as fwlite
#events = fwlite.Events("file:step3.root")
events = fwlite.Events("file:step3_fullTracking_v0.root")
tracks = fwlite.Handle("std::vector<reco::Track>")

for i, event in enumerate(events):
    if i >= 5: break            # only the first 5 events
    print "Event", i
    event.getByLabel("generalTracks", tracks)

    numTotal = tracks.product().size()
    numLoose = 0
    numTight = 0
    numHighPurity = 0

    for j, track in enumerate(tracks.product()):
        print "    Track", j,
        print "\t charge/pT: %.3f" %(track.charge()/track.pt()),
        print "\t phi: %.3f" %track.phi(),
        print "\t eta: %.3f" %track.eta(),
        print "\t dxy: %.4f" %track.dxy(),
        print "\t dz: %.4f"  %track.dz()

        if track.quality(track.qualityByName("loose")):      numLoose      += 1
        if track.quality(track.qualityByName("tight")):      numTight      += 1
        if track.quality(track.qualityByName("highPurity")): numHighPurity += 1

        print "    Track", j,
        print "\t charge/pT: %.3f" %(track.charge()/track.pt()),
        print "\t phi: %.3f" %track.phi(),
        print "\t eta: %.3f" %track.eta(),
        print "\t dxy: %.4f" %track.dxy(),
        print "\t dz: %.4f"  %track.dz(),
        print "\t nHits: %s" %track.numberOfValidHits(), "(%s P+ %s S)"%(track.hitPattern().numberOfValidPixelHits(),track.hitPattern().numberOfValidStripHits()),
        print "\t algo: %s"  %track.algoName(),

    print "Event", i,
    print "numTotal:", numTotal,
    print "numLoose:", numLoose, "(%.1f %%)"%(float(numLoose)/numTotal*100),
    print "numTight:", numTight, "(%.1f %%)"%(float(numTight)/numTotal*100),
    print "numHighPurity:", numHighPurity, "(%.1f %%)"%(float(numHighPurity)/numTotal*100)
