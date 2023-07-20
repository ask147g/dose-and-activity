#ifndef PARALLEL_WORLD
#define PARALLEL_WORLD_HH

#include "G4VUserParallelWorld.hh"
#include "globals.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSDoseDeposit.hh"
#include "G4RotationMatrix.hh"

#include "EffectiveDose.hh"
#include "InducedActivity.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

class ParallelWorldMany : public G4VUserParallelWorld {
public:
    ParallelWorldMany(G4String, G4ThreeVector world, G4ThreeVector det);
    virtual ~ParallelWorldMany();

    virtual void Construct();
    virtual void ConstructSD();

private:
    void SetupGeometry();
    void SetupDetectors();

    G4LogicalVolume *waterLogic;
    G4ThreeVector worldSize;
    G4ThreeVector detectorSize;
};

#endif