// -----------------------------------------------------------------------------
//  G4Basic | PrimaryGeneration.cpp
//
//  Class for the definition of the primary generation action.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "PrimaryGeneration.h"

#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>
#include <G4IonTable.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>
#include <G4Event.hh>
#include <G4Electron.hh>
#include <G4MuonPlus.hh>
#include <G4Proton.hh>

#include <iostream>
#include <fstream>

#include <G4GenericMessenger.hh>
#include <Randomize.hh>

#include <stdlib.h>
#include <math.h>

// MARLEY includes
#include "marley/Event.hh"
#include "marley/Particle.hh"
#include "marley/JSONConfig.hh"

#include "G4PhysicalConstants.hh"

using namespace std;


PrimaryGeneration::PrimaryGeneration():
  G4VUserPrimaryGeneratorAction()
{
  msg_ = new G4GenericMessenger(this, "/Inputs/", "Control commands of the ion primary generator.");
  msg_->DeclareProperty("Particle_Type", Particle_Type_,  "which particle?");
  msg_->DeclareProperty("MARLEY_json", MARLEY_json_,  "marley json file?");

  //msg_->DeclareProperty("Particle_energy", Particle_Energy_,  "Energy of the particle.");
  // get dictionary of particles
  particle_table_ = G4ParticleTable::GetParticleTable();

}


PrimaryGeneration::~PrimaryGeneration()
{
  delete msg_;
}


void PrimaryGeneration::GeneratePrimaries(G4Event* event)
{
  if (Particle_Type_ ==  "Ar39")
  {
    G4ParticleDefinition* pdef = G4IonTable::GetIonTable()->GetIon(18, 39, 0.); // Ar39
    if (!pdef)G4Exception("SetParticleDefinition()", "[IonGun]",FatalException, " can not create ion ");

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef);
    particle->SetMomentumDirection(G4ThreeVector(0.,1.,0.));
    particle->SetKineticEnergy(1.*eV); // just an ion sitting

    double Ran_X = G4UniformRand()*(1.0/2)*m;
    double Ran_Y = G4UniformRand()*(1.0/2)*m;
    double Ran_Z = G4UniformRand()*(5.0/2)*m;
    if (G4UniformRand()>0.5){ Ran_X *= -1; }
    if (G4UniformRand()>0.5){ Ran_Y *= -1; }
    if (G4UniformRand()>0.5){ Ran_Z *= -1; }
    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(Ran_X,Ran_Y,Ran_Z), 0.);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);
  }
  else if (Particle_Type_ ==  "Muon")
  {
    double Ran_X = G4UniformRand()*(1.0/2)*m;
    double Ran_Y = G4UniformRand()*(1.0/2)*m;
    double Ran_Z = G4UniformRand()*(5.0/2)*m;
    if (G4UniformRand()>0.5){ Ran_X *= -1; }
    if (G4UniformRand()>0.5){ Ran_Y *= -1; }
    if (G4UniformRand()>0.5){ Ran_Z *= -1; }
    G4ParticleDefinition* pdef = G4MuonPlus::Definition();

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef);
    particle->SetMomentumDirection(G4ThreeVector(0.,1.,0.));
    particle->SetKineticEnergy(10.*GeV); 

    
    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(0, -(1.0/2)*m ,Ran_Z), 0.);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);
  }
  else if (Particle_Type_ ==  "Proton")
  {

    double Ran_X = G4UniformRand()*(1.0/2)*m;
    double Ran_Y = G4UniformRand()*(1.0/2)*m;
    double Ran_Z = G4UniformRand()*(5.0/2)*m;
    if (G4UniformRand()>0.5){ Ran_X *= -1; }
    if (G4UniformRand()>0.5){ Ran_Y *= -1; }
    if (G4UniformRand()>0.5){ Ran_Z *= -1; }
    double NorXY = sqrt(Ran_X*Ran_X+Ran_Y*Ran_Y) ;

    G4ParticleDefinition* pdef = G4Proton::Definition();

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef);
    particle->SetMomentumDirection(G4ThreeVector(Ran_X/NorXY , Ran_Y/NorXY, 0.));
    particle->SetKineticEnergy(100.*MeV); // just an ion sitting

    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(Ran_X,Ran_Y,Ran_Z), 0.);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);
  }
  else if (Particle_Type_ ==  "Electron")
  {
    G4ParticleDefinition* pdef = G4Electron::Definition();

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef);
    particle->SetMomentumDirection(G4ThreeVector(0.,1.,0.));
    particle->SetKineticEnergy(3.*MeV); 

    
    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(0, 0 ,0.5), 0.);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);
  }

  else if (Particle_Type_ ==  "MARLEY")
  {
    //chdir("/Users/austinmcdonald/software/MARLEY/marley/build");
    
    //for (int i = 0; i < 50; i++) {G4cout << "MARLEY ON "<< MARLEY_json_ << G4endl;}
    marley::JSONConfig config( MARLEY_json_ );
    marley_generator_= config.create_generator();
 
    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(0., 0. ,0.), 0.);

    // Generate a new MARLEY event using the owned marley::Generator object
    marley::Event ev = marley_generator_.create_event();
    
    // Loop over each of the final particles in the MARLEY event
    for ( const auto& fp : ev.get_final_particles() ) {
      // Convert each one from a marley::Particle into a G4PrimaryParticle.
      // Do this by first setting the PDG code and the 4-momentum components.
	    
	   // get dictionary of particles if necessary
      if (particle_table_ == 0)
      {
        particle_table_ = G4ParticleTable::GetParticleTable();
      }
      // initialize particle definition
      G4ParticleDefinition* pdef;
      // get PDG code of marley::Particle
      int const pdg_code = fp->pdg_code();
      if (pdg_code == 0)
      {
        pdef = particle_table_->FindParticle("opticalphoton");
      }
      else
      {
        pdef = particle_table_->FindParticle(pdg_code);
      }
      // if the particle is a nucleus
      if (pdg_code > 1000000000)
      {
        if (!pdef)
        {
          int const Z = (pdg_code % 10000000) / 10000; // atomic number
          int const A = (pdg_code % 10000) / 10; // mass number
          pdef = particle_table_->GetIonTable()->GetIon(Z, A, 0.);
        }
      } // if the particle is a nucleus
      if (pdef == 0)
      {
        std::string message = "\nLine "
                            + std::to_string(__LINE__)
                            + " of file "
                            + __FILE__
                            + "\n\nUnknown PDG code: "
                            + std::to_string(pdg_code)
                            + "\n";
        G4Exception("PrimaryGeneration::GeneratePrimaries", "Error",
                    FatalException, message.c_str());
      }
      G4PrimaryParticle* particle = new G4PrimaryParticle(pdef,
                                                          fp->px(),
                                                          fp->py(),
                                                          fp->pz(),
							 fp->total_energy() );

	
	// Write the MARLEY output to a file
	ofstream marleyoutput;
	marleyoutput.open("/n/home11/ntata/projects/Q_PIX_GEANT4/build/marley_out.txt",ios::app);
	marleyoutput << fp->pdg_code() << "\n";
	marleyoutput.close();



      // Also set the charge of the G4PrimaryParticle appropriately
      particle->SetCharge( fp->charge() );

      // Add the fully-initialized G4PrimaryParticle to the primary vertex
      vertex->SetPrimary( particle );
    }
    
    // The primary vertex has been fully populated with all final-state particles
    // from the MARLEY event. Add it to the G4Event object so that Geant4 can
    // begin tracking the particles through the simulated geometry.
    event->AddPrimaryVertex( vertex );

  }

  else
  {
    exit (EXIT_FAILURE);
    //G4Exception(FatalException, " Pick a defined particle... ");
  }
  

}
