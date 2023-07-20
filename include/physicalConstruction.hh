#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4RotationMatrix.hh"
#include "G4PVReplica.hh"

#include "EffectiveDose.hh"
#include "InducedActivity.hh"

#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"

#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"

#include <vector>

class PhysicalConstruction : public G4VUserDetectorConstruction {
public:
	PhysicalConstruction(G4ThreeVector world);
	~PhysicalConstruction();
	
	virtual G4VPhysicalVolume *Construct();
private:
	G4VPhysicalVolume* SetupGeometry();
	G4VPhysicalVolume* BuildMotherVolume();
	void BuildWorldConstruction();

	void BuildTarget();
	void BuildWalls();
	void BuildWall();
	void BuildProtection();
	

	G4LogicalVolume* logicWorld;
	G4ThreeVector worldSize;
};

#endif