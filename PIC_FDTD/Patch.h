//! \file
//! \brief Definition of Patch class 
//! \author Rahul Kalampattel
//! \date Last updated October 2017

#pragma once

#include <iostream>

#include "ChargeProjector.h"
#include "FDTD.h"
#include "FieldInterpolator.h"
#include "FieldSolver.h"
#include "MCC.h"
#include "Mesh.h"
#include "Parameters.h"
#include "ParticlePusher.h"
#include "VectorParticle.h"

using namespace std;

//! \class Patch
//! \brief Definition
class Patch
{
private:
	std::string tecplotMesh = "tecplotMeshC";
	std::string tecplotSolution_T = "tecplotSolutionC_T";

public:
	Patch();										//!< Default constructor
	Patch(Parameters *parametersList, int patchID);	//!< Constructor
	~Patch();										//!< Destructor
	void generateOutput(std::string solutionName, vector2D data, double t);		//!< Generate Tecplot output
	void startPIC();								//!< Start the PIC loop within a Patch object
	
	int patchID;
	double t = 0.0;
	Parameters parametersList;
	Mesh mesh;
	VectorParticle particlesVector;
};