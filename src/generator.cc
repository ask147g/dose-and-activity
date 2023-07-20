#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator() 
	: gun(new G4GeneralParticleSource) {
}

MyPrimaryGenerator::~MyPrimaryGenerator() {
	delete gun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
	gun->GeneratePrimaryVertex(anEvent);
}