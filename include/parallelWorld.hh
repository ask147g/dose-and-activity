#ifndef PARALLEL_WORLD
#define PARALLEL_WORLD_HH

#include "G4VUserParallelWorld.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"

#include "EffectiveDose.hh"
#include "InducedActivity.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

class ParallelWorld : public G4VUserParallelWorld {
public:
    ParallelWorld(G4String name);
    virtual ~ParallelWorld();

    virtual void Construct();
    virtual void ConstructSD();

private:
    void SetupGeometry();
    void SetupDetectors();

    G4LogicalVolume *waterLogic;
};

#endif