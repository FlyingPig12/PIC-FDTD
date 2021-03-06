//! \file
//! \brief Definition of Mesh class 
//! \author Rahul Kalampattel
//! \date Last updated April 2018

#pragma once

#include "VectorCell.h"
#include "VectorFace.h"
#include "VectorGhost.h"
#include "VectorNode.h"

class Parameters;	// Forward declaration to resolve circular dependency 

// TODO: Make Mesh a base class, and have different derived classes for the PIC 
// and FDTD meshes (e.g. don't need to add particles to the FDTD mesh, and the 
// PIC mesh doesn't need to support interpolation like the FDTD mesh).

//! \class Mesh
//! \brief Compiled mesh properties extracted from gridinfo and gridgeo
class Mesh
{
public:
	// Data members
	int numCells;							//!< Number of cells
	int numFaces;							//!< Number of faces
	int numGhost;							//!< Number of ghost cells
	int numNodes;							//!< Number of nodes
	int dimension;							//!< Dimension of mesh (2D)
	int numRows;							//!< Number of rows of cells
	int numColumns;							//!< Number of columns of cells
	double h;								//!< Cell width/length (assume uniform)
	VectorCell cellsVector;					//!< Vector of cells
	VectorFace facesVector;					//!< Vector of faces
	VectorGhost ghostVector;				//!< Vector of ghost cells
	VectorNode nodesVector;					//!< Vector of nodes


	// Constructor/destructor
	Mesh();									//!< Default constructor
	Mesh(Parameters *localParametersList, 
		std::string type);					//!< Constructor
	~Mesh();								//!< Destructor


	// Methods
	void addParticlesToCell(int cellID, 
		int particleID);					//!< Assign particle IDs to a cell
	void removeParticlesFromCell(int cellID,
		int particleID);					//!< Remove particle IDs from a cell
};