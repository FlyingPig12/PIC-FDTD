//! \file
//! \brief Implementation of ChargeProjector class 
//! \author Rahul Kalampattel
//! \date Last updated March 2018

#include "ChargeProjector.h"

// Default constructor
ChargeProjector::ChargeProjector()
{
}

// Constructor
ChargeProjector::ChargeProjector(Parameters *parametersList,
	Mesh *mesh, VectorParticle *particlesVector)
{
	// Set charge at all nodes to zero at the start of each step
	mesh->nodesVector.clearCharge();

	double hSquared = mesh->h * mesh->h;

	// Project charge to nodes
	for (int i = 0; i < particlesVector->numParticles; i++)
	{
		// TODO: Can change all of the below to references to avoid copying large 
		// amounts of data for each calculation

		int cellID = particlesVector->particleVector[i].cellID - 1;
		int nodeID_0 = mesh->cellsVector.cells[cellID].connectivity.nodeIDs[0] - 1;
		int nodeID_1 = mesh->cellsVector.cells[cellID].connectivity.nodeIDs[1] - 1;
		int nodeID_2 = mesh->cellsVector.cells[cellID].connectivity.nodeIDs[2] - 1;
		int nodeID_3 = mesh->cellsVector.cells[cellID].connectivity.nodeIDs[3] - 1;

		double left = mesh->cellsVector.cells[cellID].left;
		double right = mesh->cellsVector.cells[cellID].right;
		double top = mesh->cellsVector.cells[cellID].top;
		double bottom = mesh->cellsVector.cells[cellID].bottom;
		
		double x1 = particlesVector->particleVector[i].position[0];
		double x2 = particlesVector->particleVector[i].position[1];

		std::string firstNodePosition = mesh->cellsVector.cells[cellID].firstNodePosition;
		double charge = particlesVector->particleVector[i].basic.q;

		if (firstNodePosition == "BL")
		{
			mesh->nodesVector.nodes[nodeID_0].charge += charge * (right - x1) * (top - x2) / hSquared;
			mesh->nodesVector.nodes[nodeID_1].charge += charge * (x1 - left) * (top - x2) / hSquared;
			mesh->nodesVector.nodes[nodeID_2].charge += charge * (x1 - left) * (x2 - bottom) / hSquared;
			mesh->nodesVector.nodes[nodeID_3].charge += charge * (right - x1) * (x2 - bottom) / hSquared;

		}
		else if (firstNodePosition == "BR")
		{
			mesh->nodesVector.nodes[nodeID_0].charge += charge * (x1 - left) * (top - x2) / hSquared;
			mesh->nodesVector.nodes[nodeID_1].charge += charge * (x1 - left) * (x2 - bottom) / hSquared;
			mesh->nodesVector.nodes[nodeID_2].charge += charge * (right - x1) * (x2 - bottom) / hSquared;
			mesh->nodesVector.nodes[nodeID_3].charge += charge * (right - x1) * (top - x2) / hSquared;
		}
		else if (firstNodePosition == "TR")
		{
			mesh->nodesVector.nodes[nodeID_0].charge += charge * (x1 - left) * (x2 - bottom) / hSquared;
			mesh->nodesVector.nodes[nodeID_1].charge += charge * (right - x1) * (x2 - bottom) / hSquared;
			mesh->nodesVector.nodes[nodeID_2].charge += charge * (right - x1) * (top - x2) / hSquared;
			mesh->nodesVector.nodes[nodeID_3].charge += charge * (x1 - left) * (top - x2) / hSquared;
		}
		else if (firstNodePosition == "TL")
		{	
			mesh->nodesVector.nodes[nodeID_0].charge += charge * (right - x1) * (x2 - bottom) / hSquared;
			mesh->nodesVector.nodes[nodeID_1].charge += charge * (right - x1) * (top - x2) / hSquared;
			mesh->nodesVector.nodes[nodeID_2].charge += charge * (x1 - left) * (top - x2) / hSquared;
			mesh->nodesVector.nodes[nodeID_3].charge += charge * (x1 - left) * (x2 - bottom) / hSquared;
		}
	}

	// Calculate charge density (charge / cell area)
	for (int i = 0; i < mesh->numNodes; i++)
	{
		// For cylindrical case, need to account for changing cell volume
		if (parametersList->axisymmetric == true)
		{
			int bottomNodeID = mesh->nodesVector.nodes[i].bottomNodeID;
			if (bottomNodeID > 0)
			{
				mesh->nodesVector.nodes[i].rho = mesh->nodesVector.nodes[i].charge /
					std::_Pi * mesh->h * (mesh->nodesVector.nodes[i].geometry.X.element(1, 0) *
						mesh->nodesVector.nodes[i].geometry.X.element(1, 0) -
						mesh->nodesVector.nodes[bottomNodeID].geometry.X.element(1, 0) *
						mesh->nodesVector.nodes[bottomNodeID].geometry.X.element(1, 0));
			}
			else
			{
				// Zero cell volume at axis
				mesh->nodesVector.nodes[i].rho = 0.0;
			}
		}
		else
		{
			// Cartesian case
			mesh->nodesVector.nodes[i].rho = mesh->nodesVector.nodes[i].charge / hSquared;
		}
		
		if (parametersList->simulationType == "electron")
		{
			// Assuming a cold plasma of mobile electrons and fixed ions with equal 
			// and opposite charge, we must account for this neutralising background 
			// charge density
			mesh->nodesVector.nodes[i].rho -= ((particlesVector->numParticles * ELECTRON_CHARGE) /
				(mesh->numCells * hSquared * mesh->numNodes));
		}
		// TODO: Background charge for other simulationType == "partial"?
	}

	// TODO: Change variable names below
	// Account for periodic BCs 
	for (int i = 0; i < mesh->numNodes; i++)
	{
		if (parametersList->xBCType == "periodic")
		{
			if (mesh->nodesVector.nodes[i].boundaryType == "TL" ||
				mesh->nodesVector.nodes[i].boundaryType == "L" ||
				mesh->nodesVector.nodes[i].boundaryType == "BL")
			{
				mesh->nodesVector.nodes[i].rho = 0.5 * (mesh->nodesVector.nodes[i].rho +
					mesh->nodesVector.nodes[mesh->nodesVector.nodes[i].periodicXNodeID - 1].rho);
				mesh->nodesVector.nodes[mesh->nodesVector.nodes[i].periodicXNodeID - 1].rho =
					mesh->nodesVector.nodes[i].rho;
			}
		}
		if (parametersList->yBCType == "periodic")
		{
			if (mesh->nodesVector.nodes[i].boundaryType == "TL" ||
				mesh->nodesVector.nodes[i].boundaryType == "T" ||
				mesh->nodesVector.nodes[i].boundaryType == "TR")
			{
				mesh->nodesVector.nodes[i].rho = 0.5 * (mesh->nodesVector.nodes[i].rho +
					mesh->nodesVector.nodes[mesh->nodesVector.nodes[i].periodicYNodeID - 1].rho);
				mesh->nodesVector.nodes[mesh->nodesVector.nodes[i].periodicYNodeID - 1].rho =
					mesh->nodesVector.nodes[i].rho;
			}
		}
	}
	parametersList->logBrief("Charge projector exited", 1);
}


// Destructor
ChargeProjector::~ChargeProjector()
{
}