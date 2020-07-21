#include "TrackingAction.h"
#include "G4TrackingManager.hh"
#include <iostream>
#include <fstream>
using namespace std;

TrackingAction::TrackingAction()
{
}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{   ofstream myfile;
    myfile.open ("~/projects/Q_PIX_GEANT4/src/preoutput.txt");
    const G4ParticleDefinition* particleDefinition = track->GetDefinition();
    const G4int pdgCode = particleDefinition->GetPDGEncoding();
    const G4int trackID = track->GetTrackID();
    const G4int parentID = track->GetParentID();
    myfile << "TrackingAction::PreUserTrackingAction():"
              << "\ntrackID:  " << trackID
              << "\nparentID: " << parentID
              << "\npdgCode:  " << pdgCode
              << "\n----------------------------------------------------------"
    myfile.close()     
}
void TrackingAction::PostUserTrackingAction(const G4Track* track)
{   ofstream myfile;
    myfile.open("~/projects/Q_PIX_GEANT4/src/postoutput.txt")
    const G4ParticleDefinition* particleDefinition = track->GetDefinition();
    const G4int pdgCode = particleDefinition->GetPDGEncoding();
    const G4int trackID = track->GetTrackID();
    const G4int parentID = track->GetParentID();
    const G4String process = track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    myfile << "TrackingAction::PostUserTrackingAction():"
              << "\ntrackID:  " << trackID
              << "\nparentID: " << parentID
              << "\npdgCode:  " << pdgCode
              << "\nprocess:  " << process
              << "\n----------------------------------------------------------"
    myfile.close()
}
