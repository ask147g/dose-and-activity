#include "parallelWorldMany.hh"

ParallelWorldMany::ParallelWorldMany(G4String name, 
    G4ThreeVector world, G4ThreeVector det)
        :G4VUserParallelWorld(name), worldSize(world), detectorSize(det) {
}

ParallelWorldMany::~ParallelWorldMany() {
}

void ParallelWorldMany::Construct() {
    SetupGeometry();
}

void ParallelWorldMany::ConstructSD() {
    SetupDetectors();
}

void ParallelWorldMany::SetupGeometry() {
    G4VPhysicalVolume* globalWorld = GetWorld();
    G4LogicalVolume* logicWorld = globalWorld->GetLogicalVolume();

    G4Box *waterBox = 
        new G4Box(
            "waterBox", 
            detectorSize.x(), 
            detectorSize.y(), 
            detectorSize.z());

    waterLogic = 
        new G4LogicalVolume(
            waterBox, 
            0, 
            "waterLogic");

    int copy = 0;
    for (int iX = 0; iX < worldSize.x()/detectorSize.x(); iX++) {
        for (int iY = 0; iY < worldSize.y()/detectorSize.y(); iY++) {
            for (int iZ = 0; iZ < worldSize.z()/detectorSize.z(); iZ++) {
                new G4PVPlacement(
                    new G4RotationMatrix(0, 0., 0.), 
		        	G4ThreeVector(-worldSize.x()+detectorSize.x()+iX*(2*detectorSize.x()), 
                        -worldSize.y()+detectorSize.y()+iY*(2*detectorSize.y()), 
                        -worldSize.z()+detectorSize.z()+iZ*(2*detectorSize.z())), 
		        	waterLogic, 
		        	"physWorld_main", 
		        	logicWorld, 
		        	false, 
		        	copy, 
		        	false);
                copy++;
            }
        } 
    }
}

void ParallelWorldMany::SetupDetectors() {
    G4MultiFunctionalDetector* det = new G4MultiFunctionalDetector("doseDetector");
    G4VPrimitiveScorer* doseRate;
    G4VPrimitiveScorer* activity;
    doseRate = new EffectiveDose("Edep");
    activity = new InducedActivity("Activity");
    det->RegisterPrimitive(doseRate);
    det->RegisterPrimitive(activity);
    G4SDManager::GetSDMpointer()->AddNewDetector(det);
    SetSensitiveDetector(waterLogic, det);
}