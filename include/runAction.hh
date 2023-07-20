#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"

#include "run.hh"

#include <fstream>

class G4Run;

class UserRunAction : public G4UserRunAction {
public:
    UserRunAction();
    virtual ~UserRunAction();

    G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void EndOfRunAction(const G4Run* aRun);
private:
};

#endif