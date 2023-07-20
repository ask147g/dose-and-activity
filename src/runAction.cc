#include "runAction.hh"

UserRunAction::UserRunAction() {
}

UserRunAction::~UserRunAction() {
}

G4Run* UserRunAction::GenerateRun() {
    return new UserRun;
}

void UserRunAction::BeginOfRunAction(const G4Run* aRun) {

}

void UserRunAction::EndOfRunAction(const G4Run* aRun) {
    if(IsMaster()) {
        const UserRun* theRun = (const UserRun*)aRun;
        std::ofstream out;
        out.open("dose.csv");

        std::ofstream outActivity;
        outActivity.open("activity.csv");

        if (out.is_open())
        {
            // amount of detectors there
            for (int x = 0; x < 20; x++) {
                for (int y = 0; y < 20; y++) {
                    for (int z = 0; z < 20; z++) {
                        out << theRun->GetTotalE(x, y, z)/CLHEP::joule*CLHEP::kilogram/1.e-12 << std::endl;

                        auto nuclides = theRun->GetNuclides(x, y, z);
                        auto it = nuclides.begin();
                        for(; it != nuclides.end(); it++) {
                          outActivity << it->first.first << " " << it->first.second << " " << it->second.first << " " << it->second.second << std::endl;
                        }
                        outActivity << "#END" << std::endl;
                    }
                }
            }
        }
        out.close(); 
        outActivity.close(); 
    }
}