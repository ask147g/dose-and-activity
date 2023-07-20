#include "G4RunManagerFactory.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "QBBC.hh"
#include "QGSP_BERT_HP.hh"
#include "G4ParallelWorldPhysics.hh"

#include "physicalConstruction.hh"
#include "parallelWorld.hh"
#include "parallelWorldMany.hh"
#include "action.hh"

int main(int argc, char** argv) {
    CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine); 
    CLHEP::HepRandom::setTheSeed((unsigned)clock()); 

    G4UIExecutive* ui = nullptr;
    if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }

    auto runManager = G4RunManagerFactory::CreateRunManager();
	//G4RunManager* runManager = new G4RunManager();

    //G4ThreeVector worldSize = G4ThreeVector(10*CLHEP::cm, 10*CLHEP::cm, 10*CLHEP::cm);
    G4ThreeVector worldSize = G4ThreeVector(654./2.*CLHEP::cm, 300./2.*CLHEP::cm, 929./2.*CLHEP::cm);
	G4ThreeVector detectorSize = G4ThreeVector(654./2./20.*CLHEP::cm, 300./2./20.*CLHEP::cm, 929./2./20.*CLHEP::cm);

    PhysicalConstruction* world = new PhysicalConstruction(worldSize);
    G4String parallelWorldName = "ParallelWorld";
    //world->RegisterParallelWorld(new ParallelWorld(parallelWorldName));
    world->RegisterParallelWorld(new ParallelWorldMany(parallelWorldName, worldSize, detectorSize));
	runManager->SetUserInitialization(world);
    
    auto physics = new QGSP_BERT_HP();
    
    physics->RegisterPhysics(new G4ParallelWorldPhysics(parallelWorldName));
    runManager->SetUserInitialization(physics);
	runManager->SetUserInitialization(new MyActionInitialization());
	
	runManager->Initialize();
	
	G4VisManager *visManager = new G4VisExecutive();
	visManager->Initialize();
	
	G4UImanager *UImanager = G4UImanager::GetUIpointer();
	
	if ( ! ui ) {
    // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else {
    // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
	
    delete visManager;
    delete runManager;
	return 0;
}