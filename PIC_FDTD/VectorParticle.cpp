//! \file
//! \brief Implementation of VectorParticle class 
//! \author Rahul Kalampattel
//! \date Last updated October 2017

#include "VectorParticle.h"

VectorParticle::VectorParticle()
{
}

VectorParticle::VectorParticle(Parameters *localParametersList)
{
	localParametersList->logMessages("Creating particles vector...");
	for (int i = 0; i < localParametersList->particlesPerPatch; i++)
	{
		Particle particle(localParametersList);
		particleVector.push_back(particle);
	}
}


VectorParticle::~VectorParticle()
{
}