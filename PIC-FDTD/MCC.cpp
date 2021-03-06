//! \file
//! \brief Implementation of MCC class 
//! \author Rahul Kalampattel
//! \date Last updated April 2018

#include "MCC.h"

// Default constructor
MCC::MCC()
{
}


// Constructor
MCC::MCC(Parameters *parametersList, Mesh *mesh, VectorParticle *particlesVector)
{
	// TODO: Check that it is appropriate to use MCC (compare particle densities,
	// collision frequencies, etc.)

	for (int i = 0; i < particlesVector->numParticles; i++)
	{	
		double sigma, targetDensity;
		
		// Collision cross section
		if (parametersList->propellant == "xenon")
		{
			// TODO: Needs to be calculated with relative velocity, can only
			// use magnitude it velocity difference is sufficiently high
			sigma = 15.1262 - 0.8821 * 
				log(particlesVector->particleVector[i].velocityMagnitude());
		}
		
		// Check simulation type
		if (parametersList->simulationType == "full")
		{
			// TODO: Collisions for all three types
		}
		else if (parametersList->simulationType == "partial")
		{
			// Neutral
			if (particlesVector->particleVector[i].basic.type == 0)
			{
				// Target (ion) density
				// TODO: Target density should be for TARGET species only (i.e.
				// only count particles of a specific kind, rather than all particles
				// in the cell. Otherwise, use some sort of distribution to calculate
				// the correct density.
				targetDensity = static_cast<double>(mesh->cellsVector.cells[particlesVector->particleVector[i].cellID - 1].listOfParticles.size()) /
					(mesh->h * mesh->h);
			}
			
			// Ion
			else if (particlesVector->particleVector[i].basic.type == 0)
			{
				// Target (neutral) density
			}
		}
		else if (parametersList->simulationType == "electron")
		{
			// TODO: Collisions for electrons
			targetDensity = static_cast<double>(mesh->cellsVector.cells[particlesVector->particleVector[i].cellID - 1].listOfParticles.size()) /
				(mesh->h * mesh->h);
		}

		// TODO: Again, in general need to use the relative velocity
		// TODO: Separate time step for collisions?
		double collisionProbability = 1 - exp(-targetDensity * sigma * parametersList->timeStep *
			particlesVector->particleVector[i].velocityMagnitude());

		// Initialise random number generator, distribution in range [0, 1000000]
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1000000);

		double collisionRand = dist(rng) / (double)1000000;
		double collisionRandType = dist(rng) / (double)1000000;

		if (collisionProbability > collisionRand)
		{
			// TODO: Process specific collision handler, e.g. for CEX collisions, replace
			// velocities of the collided particle with values from a Maxwellian distribution.
			// 
			// Types of collisions include:
			// Ionisation:		  Xe   + e-	   -> Xe+   + 2e-
			// Recombination:	  Xe+  + e-	   -> Xe
			// Charge exchange:	  Xe_f + Xe+_s -> Xe+_f + Xe_s
			// Momentum exchange: Xe_f + Xe_s  -> Xe_s  + Xe_f (not well supported by MCC?)
			//
			// Collisions modelled depends on simulation type:
			// electron: ionisation (+1 electron), recombination (-1 electron)
			// partial: ionisation (-1 neutral, +1 ion), recombination (+1 neutral, -1 ion), charge exchange
			// full: ionisation (-1 neutral, +1 ion/electron), recombination (+1 neutral, -1 ion/electron), charge exchange
			//
			// Additionally, particle production occurs as a result of inductive or
			// electron heating (RF thruster)

			// particlesVector->addParticleToSim(parametersList, mesh, 1, "electron");
			// particlesVector->removeParticleFromSim(particlesVector->particleVector[i].particleID);

		}

	}

	parametersList->logBrief("Collision handler exited", 1);
}


// Destructor
MCC::~MCC()
{
}