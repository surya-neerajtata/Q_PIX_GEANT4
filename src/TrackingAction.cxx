#include "TrackingAction.h"
#include "G4TrackingManager.hh"
#include <iostream>
#include <fstream>
using namespace std;

TrackingAction::TrackingAction()
{
}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{  // remove("/n/home11/ntata/projects/Q_PIX_GEANT4/preoutput.txt");
    ofstream myfile;
    myfile.open ("/n/home11/ntata/projects/Q_PIX_GEANT4/preoutput.txt",ios::app);
    const G4ParticleDefinition* particleDefinition = track->GetDefinition();
    const G4int pdgCode = particleDefinition->GetPDGEncoding();
    const G4int trackID = track->GetTrackID();
    const G4int parentID = track->GetParentID();
    myfile <<  trackID << " " << parentID << " " << pdgCode  << "\n";
    myfile.close()     ;
}
void TrackingAction::PostUserTrackingAction(const G4Track* track)
{   //remove("/n/home11/ntata/projects/Q_PIX_GEANT4/postoutput.txt");
    ofstream myfile;
    myfile.open("/n/home11/ntata/projects/Q_PIX_GEANT4/postoutput.txt",ios::app);
    const G4ParticleDefinition* particleDefinition = track->GetDefinition();
    const G4int pdgCode = particleDefinition->GetPDGEncoding();
    const G4int trackID = track->GetTrackID();
    const G4int parentID = track->GetParentID();
    const G4double edep = track->GetTotalEnergy();
    const G4double gtime = track->GetGlobalTime()
    const G4String process = track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    myfile <<  edep << " " << gtime << " " << trackID << " " << parentID << " " << pdgCode << " " << process << "\n";
    myfile.close();
}
