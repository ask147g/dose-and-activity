#include "physicalConstruction.hh"

PhysicalConstruction::PhysicalConstruction(G4ThreeVector world) : worldSize(world) {
}

PhysicalConstruction::~PhysicalConstruction() {
}

G4VPhysicalVolume *PhysicalConstruction::Construct() {
	G4VPhysicalVolume *physWorld = SetupGeometry();
	return physWorld;
}

G4VPhysicalVolume* PhysicalConstruction::SetupGeometry() {
    G4VPhysicalVolume *physWorld = BuildMotherVolume();
    BuildWorldConstruction();

    return physWorld;
}

G4VPhysicalVolume* PhysicalConstruction::BuildMotherVolume() {
    G4NistManager *nist = G4NistManager::Instance();
	G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");

    //G4Isotope* iso98 = new G4Isotope("Mo98", 42, 98, 98*CLHEP::g/mole);
    //
    //G4Element* Mo98 = new G4Element("isotope Mo98", "Mo" , 1);
    //Mo98->AddIsotope(iso98, 100*CLHEP::perCent);
    //
    //G4Material* matMo= new G4Material("Mo98 mat" , 10.22*CLHEP::g/cm3, 1, kStateSolid);
    //matMo->AddElement(Mo98, 1.00);

    G4Box *solidWorld = 
        new G4Box(
            "solidWorld", 
            worldSize.x(), 
            worldSize.y(), 
            worldSize.z());

	logicWorld = 
        new G4LogicalVolume(
            solidWorld, 
            vacuum, 
            "logicWorld");
	
	G4VPhysicalVolume *physWorld = 
        new G4PVPlacement(
            0, 
			G4ThreeVector(0., 0., 0.), 
			logicWorld, 
			"physWorld", 
			0, 
			false, 
			0, 
			false); 

    return physWorld;
}

void PhysicalConstruction::BuildWorldConstruction() {
    BuildWalls();
    BuildWall();
    BuildTarget();
    BuildProtection();
}

void PhysicalConstruction::BuildTarget() {
    G4NistManager *nist = G4NistManager::Instance();
    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
    G4Material *steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    G4Box *AlBox = 
        new G4Box(
            "targetAl", 
            5*CLHEP::cm, 
            5*CLHEP::cm, 
            1*CLHEP::cm);

    G4LogicalVolume *AlLogic = 
        new G4LogicalVolume(
            AlBox, 
            Al, 
            "targetAlLogic");

    G4VPhysicalVolume *AlWorld = 
        new G4PVPlacement(
            new G4RotationMatrix(0, 0., 0.), 
			G4ThreeVector((-87-75)*CLHEP::cm, 0., -300*CLHEP::cm), 
			AlLogic, 
			"physTargetAl", 
			logicWorld, 
			false, 
			0, 
			false); 

    G4Box *SteelBox = 
        new G4Box(
            "targetSteel", 
            5*CLHEP::cm, 
            5*CLHEP::cm, 
            5*CLHEP::cm);

    G4LogicalVolume *SteelLogic = 
        new G4LogicalVolume(
            SteelBox, 
            steel, 
            "targetSteelLogic");

    G4VPhysicalVolume *SteelWorld = 
        new G4PVPlacement(
            new G4RotationMatrix(0, 0., 0.), 
			G4ThreeVector((-87-75)*CLHEP::cm, 0., (-1-5-300)*CLHEP::cm), 
			SteelLogic, 
			"physTargetSteel", 
			logicWorld, 
			false, 
			0, 
			false); 
}

void PhysicalConstruction::BuildWalls() {
    G4NistManager *nist = G4NistManager::Instance(); // refactor: make global
    G4Material *concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

    G4Box *wall1Box = 
        new G4Box(
            "wall1", 
            worldSize.x(), 
            worldSize.y(), 
            worldSize.z());

    G4Box *wall2Box = 
        new G4Box(
            "wall2", 
            worldSize.x()-80*CLHEP::cm/2., 
            worldSize.y()-80*CLHEP::cm/2., 
            worldSize.z()-80*CLHEP::cm/2.);

    G4VSolid* wall = 
        new G4SubtractionSolid(
            "wall",
            wall1Box,
            wall2Box,
            0,
            G4ThreeVector(0., 0., 0.));
    
    G4LogicalVolume *wallLogic = 
        new G4LogicalVolume(
            wall, 
            concrete, 
            "wallLogic");

    G4VPhysicalVolume *wallWorld = 
        new G4PVPlacement(
            new G4RotationMatrix(0, 0., 0.), 
			G4ThreeVector(0., 0., 0.), 
			wallLogic, 
			"physWall", 
			logicWorld, 
			false, 
			0, 
			false); 
}

void PhysicalConstruction::BuildWall() {
    G4NistManager *nist = G4NistManager::Instance(); // refactor: make global
    G4Material *concrete = nist->FindOrBuildMaterial("G4_CONCRETE");

    G4Box *wallBox = 
        new G4Box(
            "wallRoom", 
            (220*CLHEP::cm)/2., 
            worldSize.y()-40*CLHEP::cm, 
            (11*CLHEP::cm)/2.);
    
    G4LogicalVolume *wallLogic = 
        new G4LogicalVolume(
            wallBox, 
            concrete, 
            "wallRoomLogic");

    G4VPhysicalVolume *wallWorld = 
        new G4PVPlacement(
            new G4RotationMatrix(0, 0., 0.), 
			G4ThreeVector(-177*CLHEP::cm, 0., 67*CLHEP::cm), 
			wallLogic, 
			"physWallRoom", 
			logicWorld, 
			false, 
			0, 
			false); 

    new G4PVPlacement(
        new G4RotationMatrix(0, 0., 0.), 
		G4ThreeVector(177*CLHEP::cm, 0., 67*CLHEP::cm), 
		wallLogic, 
		"physWallRoom", 
		logicWorld, 
		false, 
		1, 
		false); 
}

void PhysicalConstruction::BuildProtection() {
    G4NistManager *nist = G4NistManager::Instance(); // refactor: make global
    G4Material *lead = nist->FindOrBuildMaterial("G4_Pb");

    G4double thickness = 0.1*CLHEP::cm;

    G4Box *protectionBox = 
        new G4Box(
            "Protection", 
            (200*CLHEP::cm)/2., 
            worldSize.y()-40*CLHEP::cm, 
            thickness/2.);
    
    G4LogicalVolume *protectionLogic = 
        new G4LogicalVolume(
            protectionBox, 
            lead, 
            "ProtectionLogic");

    G4VPhysicalVolume *protectionWorld = 
        new G4PVPlacement(
            new G4RotationMatrix(0, 0., 0.), 
			G4ThreeVector(-187*CLHEP::cm, 0., (67-5.5)*CLHEP::cm-thickness/2.), 
			protectionLogic, 
			"physProtection", 
			logicWorld, 
			false, 
			0, 
			false); 

    G4Box *protectionZBox = 
        new G4Box(
            "Protection", 
            thickness/2., 
            worldSize.y()-40*CLHEP::cm, 
            386./2.*CLHEP::cm);
    
    G4LogicalVolume *protectionZLogic = 
        new G4LogicalVolume(
            protectionZBox, 
            lead, 
            "ProtectionLogic");

    G4VPhysicalVolume *protectionZWorld = 
        new G4PVPlacement(
            new G4RotationMatrix(0, 0., 0.), 
			G4ThreeVector((-87*CLHEP::cm+thickness/2.), 0., (67-5.5-386./2.)*CLHEP::cm), 
			protectionZLogic, 
			"physProtection", 
			logicWorld, 
			false, 
			0, 
			false); 


    G4Box *protection2Box = 
        new G4Box(
            "Protection", 
            (150*CLHEP::cm)/2., 
            worldSize.y()-40*CLHEP::cm, 
            thickness/2.);
    
    G4LogicalVolume *protection2Logic = 
        new G4LogicalVolume(
            protection2Box, 
            lead, 
            "ProtectionLogic");

    G4VPhysicalVolume *protection2World = 
        new G4PVPlacement(
            new G4RotationMatrix(0, 0., 0.), 
			G4ThreeVector((-87-75)*CLHEP::cm, 0., (67-5.5-386.)*CLHEP::cm-thickness/2.), 
			protection2Logic, 
			"physProtection", 
			logicWorld, 
			false, 
			0, 
			false); 

}