#include "InducedActivity.hh"
#include "G4VProcess.hh"

#include <fstream>

InducedActivity::InducedActivity(G4String name, G4int depth)
  : G4VPrimitivePlotter(name, depth),
    HCID(-1) {
}

void InducedActivity::Initialize(G4HCofThisEvent* HCE) {
  tracker = new HitsCollection(GetMultiFunctionalDetector()->GetName(), GetName());
  
  if(HCID < 0) {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, tracker);
}

G4bool InducedActivity::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  auto particle = aStep->GetTrack()->GetParticleDefinition();
  if (aStep->GetTrack()->GetParentID() == 0) return false; // only secondary etc particles
  
  //if (!(aStep->IsFirstStepInVolume())) return false; // do not work in MT

  if (particle->GetIonLifeTime() <= 0) return false; // only radioactive ions
  if (particle->GetAtomicNumber() == 0) return false; // n, e-, etc
  
  G4double activity = particle->GetIonLifeTime()/CLHEP::second;
  G4String name = particle->GetParticleName();
  G4String process = aStep->GetTrack()->GetCreatorProcess()->GetProcessName();
  G4int index = GetIndex(aStep);

  ActivityHit* newHit = new ActivityHit();
  newHit->SetName(name);
  newHit->SetLifeTime(activity);
  newHit->SetProcess(process);
  newHit->SetNumber(index);
  tracker->insert(newHit);

  return true;
}
