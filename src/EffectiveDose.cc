#include "EffectiveDose.hh"

EffectiveDose::EffectiveDose(G4String name, G4int geometry, G4int depth)
  : G4VPrimitivePlotter(name, depth),
    HCID(-1),
    EvtMap(nullptr),
    detectorGeometry(geometry) {
      new G4UnitDefinition("millisievert", "milliSv" , "Effective Dose", millisievert);
      new G4UnitDefinition("microsievert", "microSv" , "Effective Dose", microsievert);
      new G4UnitDefinition("nanosievert" , "nanoSv"  , "Effective Dose", nanosievert);
      new G4UnitDefinition("picosievert" , "picoSv"  , "Effective Dose", picosievert);
}

void EffectiveDose::Initialize(G4HCofThisEvent* HCE) {
  EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
                                    GetName());
  if(HCID < 0)
  {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*) EvtMap);
}

G4bool EffectiveDose::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  G4int pdg = aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  G4double energy = aStep->GetTrack()->GetTotalEnergy();
  G4int idx = ((G4TouchableHistory*) (aStep->GetPreStepPoint()->GetTouchable()))
                ->GetReplicaNumber(indexDepth);

  G4ThreeVector pre = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector post = aStep->GetPostStepPoint()->GetPosition();

  G4double length = std::sqrt(std::pow(pre.x()/CLHEP::cm - post.x()/CLHEP::cm, 2) + 
    std::pow(pre.y()/CLHEP::cm - post.y()/CLHEP::cm, 2) + 
    std::pow(pre.z()/CLHEP::cm - post.z()/CLHEP::cm, 2));
  
  G4double dose = ConvertDim(pdg, energy, detectorGeometry+1)*length/ComputeVolume(aStep, idx);
  G4int index = GetIndex(aStep);
  
  EvtMap->add(index, dose);

  return true;
}

G4double EffectiveDose::ComputeVolume(G4Step* aStep, G4int idx) {
  G4VSolid* solid = ComputeSolid(aStep, idx);
  return solid->GetCubicVolume()/CLHEP::cm3;
}

double EffectiveDose::ConvertDim(double pdg, double energy_MeV, int geometry) {
  double energy = energy_MeV/MeV;
  double coeff = Convert(pdg, energy, geometry);
  return coeff*picosievert;
}

double EffectiveDose::Convert(double pdg, double energy, int geometry) {
  double* array = nullptr;
    int len = 0;
    if (pdg == 22)
      {
	array = &photons[0][0];
	len = 55;
      }
    else if(pdg == 11)
      {
	array = &electrons[0][0];
	len = 49;
      }
    else if (pdg==-11)
      {
	array = &positrons[0][0];
	len =49;
      }
    else if (pdg == 13)
      {
	array = &muon_minus[0][0];
	len = 41;
      }
    else if (pdg == -13)
      {
	array = &muon_plus[0][0];
	len = 41;
      }
    else if (pdg == 211)
      {
	array = &pion_plus[0][0];
	len = 43; 
      }
    else if (pdg == -211)
      {
	array = &pion_minus[0][0];
	len =43;
      }
    else if (pdg == 2212)
      {
	array = &protons[0][0];
	len = 33;
      }
    else if (pdg ==2112)
      {
	array = &neutrons[0][0];
	len = 68;
      }
    else if (pdg == 1000020030)
      {
	array = &helium[0][0];
	len = 21;
      }
    else
      {
	return 0;
      }

    if(energy < *array)
      return *(array + geometry*len);
    
    for(auto i = 0; i < len-1; i++)
      {
	if(energy >= *(array + i) && energy < *(array + i + 1) )
	  return *(array + geometry*len + i);
      }

    return *(array + geometry*len + len - 1);
}