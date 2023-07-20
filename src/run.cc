#include "run.hh"
#include <fstream>
#include "Hit.hh"

UserRun::UserRun() {
    G4SDManager* SDMan = G4SDManager::GetSDMpointer();
    fColIDSum = SDMan->GetCollectionID("doseDetector/Edep");
    fColIDActivity = SDMan->GetCollectionID("doseDetector/Activity");
}

UserRun::~UserRun() {
}

void UserRun::RecordEvent(const G4Event* evt) {
    G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
    if(!HCE) return;
    G4THitsMap<G4double>* evtMap 
        = (G4THitsMap<G4double>*)(HCE->GetHC(fColIDSum));
    fMapSum += *evtMap;

    // Nuclides
    HitsCollection* AHC = 0;
    AHC = (HitsCollection*)(HCE->GetHC(fColIDActivity));
    for (int i = 0; i < AHC->entries(); i++) {
      if (auto key = NuclidesLot.find((*AHC)[i]->GetNumber()); key != NuclidesLot.end()) { // if replica had already be
        if (auto map = key->second.find(std::make_pair((*AHC)[i]->GetName(), 
          (*AHC)[i]->GetProcess())); map != key->second.end()) {
            ++(map->second.first);
        }
        else {
          key->second.insert({std::make_pair((*AHC)[i]->GetName(), 
            (*AHC)[i]->GetProcess()), std::make_pair(1, (*AHC)[i]->GetLifeTime())});
        }
      }
      else { // if first entry replica
        std::map<std::pair<G4String, G4String>, std::pair<G4int, G4double> > value {{std::make_pair((*AHC)[i]->GetName(), 
          (*AHC)[i]->GetProcess()), std::make_pair(1, (*AHC)[i]->GetLifeTime())}};
        NuclidesLot.insert({(*AHC)[i]->GetNumber(), value});
      }
    }
}

void UserRun::Merge(const G4Run * aRun) {
  const UserRun * localRun = static_cast<const UserRun *>(aRun);
  // Dose Rate
  fMapSum += localRun->fMapSum;

  // Nuclides
  auto itr = localRun->NuclidesLot.begin();
  for(itr; itr != localRun->NuclidesLot.end(); itr++) {
    if (auto key = NuclidesLot.find(itr->first); key != NuclidesLot.end()) { // if replica had already be
      auto itr2 = itr->second.begin();
      for (itr2; itr2 != itr->second.end(); itr2++) { // check processes
        if (auto map = key->second.find(itr2->first); map != key->second.end()) {
          map->second.first += itr2->second.first; // same particle and process found
        }
        else {
          key->second.insert({itr2->first, itr2->second}); // had not such as particle and process
        }
      }
    }
    else { // if first entry replica
      NuclidesLot.insert({itr->first, itr->second});
    }
  }
  G4Run::Merge(aRun);
}

std::map<std::pair<G4String, G4String>, std::pair<G4int, G4double> > UserRun::GetNuclides(int xx, int yy, int zz) const {
  auto it = NuclidesLot.begin();
  for(; it != NuclidesLot.end(); it++) {
    const int x = (it->first / 400);
    const int y = (it->first - x*400) / 20;
    const int z = it->first - x* 400 - y*20;
    if ((x == xx) && (y == yy) && (z == zz)) return it->second;
  }
  std::map<std::pair<G4String, G4String>, std::pair<G4int, G4double> > zero = {{std::make_pair("-", "-"), std::make_pair(0, 0)}};
  return zero;
}

// amount of detectors
G4double UserRun::GetTotalDose(int xx, int yy, int zz, const G4THitsMap<G4double> &map) const {
  if(map.GetSize()==0) return 0;
  std::map<G4int,G4double*>::iterator itr = map.GetMap()->begin();
  for(; itr != map.GetMap()->end(); itr++) {
    const int x = (itr->first / 400);
    const int y = (itr->first - x*400) / 20;
    const int z = itr->first - x* 400 - y*20;
    if ((x == xx) && (y == yy) && (z == zz)) return *(itr->second);
  }
  return 0;
}