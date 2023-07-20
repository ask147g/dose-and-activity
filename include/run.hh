#ifndef RUN_HH
#define RUN_HH

#include "globals.hh"
#include "G4Run.hh"
#include "G4Event.hh"

#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "G4HCofThisEvent.hh"

#include <map>
#include <utility>

class G4Event;

class UserRun : public G4Run {
public:
    UserRun();
    virtual ~UserRun();
    
    virtual void RecordEvent(const G4Event*);
    virtual void Merge(const G4Run*);

    G4double GetTotalE(int xx, int yy, int zz) const    { return GetTotalDose(xx, yy, zz, fMapSum); }
    std::map<std::pair<G4String, G4String>, std::pair<G4int, G4double> > GetNuclides(G4int x, G4int y, G4int z) const;
private:
    G4int fColIDSum;
    G4THitsMap<G4double> fMapSum;

    G4int fColIDActivity;

    std::map<G4int, std::map<std::pair<G4String, G4String>, std::pair<G4int, G4double> > > NuclidesLot;

    G4double GetTotalDose(int x, int y, int z, const G4THitsMap<G4double> &map) const;
};

#endif