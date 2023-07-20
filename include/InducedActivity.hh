#ifndef INDUCED_ACTIVITY_HH
#define INDUCED_ACTIVITY_HH

#include "G4VPrimitivePlotter.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"
#include "G4VCrossSectionDataSet.hh"

#include "Hit.hh"

class InducedActivity : public G4VPrimitivePlotter {
public:
	InducedActivity(G4String name, G4int depth = 0);
	~InducedActivity() override = default;
	
	void Initialize(G4HCofThisEvent*);

private:
	G4int HCID;
	HitsCollection* tracker; 

protected:
	G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
};

#endif