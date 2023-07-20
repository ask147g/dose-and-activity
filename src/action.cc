#include "action.hh"

MyActionInitialization::MyActionInitialization(){
}

MyActionInitialization::~MyActionInitialization() {
}

void MyActionInitialization::Build() const {
	SetUserAction(new MyPrimaryGenerator());
	SetUserAction(new UserRunAction());
}

void MyActionInitialization::BuildForMaster() const
{
  G4UserRunAction* run_action = new UserRunAction;
  SetUserAction(run_action);
}