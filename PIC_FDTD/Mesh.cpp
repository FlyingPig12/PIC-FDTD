//! \file
//! \brief Implementation of Mesh class 
//! \author Rahul Kalampattel
//! \date Last updated November 2017

#include "Parameters.h"
#include "Mesh.h"

// Default constructor
Mesh::Mesh()
{
}


// Constructor
Mesh::Mesh(Parameters *localParametersList)
{
	// Extract data from gridinfo
	numCells = localParametersList->gridinfo.NCM;
	numFaces = localParametersList->gridinfo.NFM;
	numGhost = localParametersList->gridinfo.NGM;
	numNodes = localParametersList->gridinfo.NNM;
	dimension = localParametersList->gridinfo.DIM;
	
	// Extract (vector) data from gridgeo
	cellsVector.allocate(localParametersList->gridgeo.cells);
	facesVector.allocate(localParametersList->gridgeo.faces);
	ghostVector.allocate(localParametersList->gridgeo.ghost);
	nodesVector.allocate(localParametersList->gridgeo.nodes);

	// Find boundaries of each cell and identify adjacent/neighbouring cells
	for (int i = 0; i < cellsVector.cells.size(); i++)
	{
		// Only need to check two opposite nodes to find cell boundaries, and 
		// since nodes are ordered anticlockwise, this is satisfied by the first
		// and third node, hence the j indices used
		for (int j = 0; j < 3; j += 2)	
		{
			// Node vector is indexed from 0, nodeIDs from 1, hence the index shift (-1) below
			int nodeID = cellsVector.cells[i].connectivity.nodeIDs[j] - 1;
			double x = nodesVector.nodes[nodeID].geometry.X.element(0, 0);
			double y = nodesVector.nodes[nodeID].geometry.X.element(1, 0);

			// Identify cell boundaries
			if (j == 0)
			{
				cellsVector.cells[i].left = x;
				cellsVector.cells[i].right = x;
				cellsVector.cells[i].top = y;
				cellsVector.cells[i].bottom = y;
			}
			else
			{
				if (x < cellsVector.cells[i].left)
				{
					cellsVector.cells[i].left = x;
				}
				else if (x > cellsVector.cells[i].right)
				{
					cellsVector.cells[i].right = x;
				}
				if (y < cellsVector.cells[i].bottom)
				{
					cellsVector.cells[i].bottom = y;
				}
				else if (y > cellsVector.cells[i].top)
				{
					cellsVector.cells[i].top = y;
				}
			}
		}

		// Get position of first node again
		int nodeID = cellsVector.cells[i].connectivity.nodeIDs[0] - 1;
		double x = nodesVector.nodes[nodeID].geometry.X.element(0, 0);	
		double y = nodesVector.nodes[nodeID].geometry.X.element(1, 0);
		
		// IDs of faces and their respective adjacent cells
		int faceID_1 = cellsVector.cells[i].connectivity.faceIDs[0] - 1;
		int leftCell_1 = facesVector.faces[faceID_1].connectivity.cl[0] - 1;
		int rightCell_1 = facesVector.faces[faceID_1].connectivity.cr[0] - 1;

		int faceID_2 = cellsVector.cells[i].connectivity.faceIDs[1] - 1;
		int leftCell_2 = facesVector.faces[faceID_2].connectivity.cl[0] - 1;
		int rightCell_2 = facesVector.faces[faceID_2].connectivity.cr[0] - 1;

		int faceID_3 = cellsVector.cells[i].connectivity.faceIDs[2] - 1;
		int leftCell_3 = facesVector.faces[faceID_3].connectivity.cl[0] - 1;
		int rightCell_3 = facesVector.faces[faceID_3].connectivity.cr[0] - 1;

		int faceID_4 = cellsVector.cells[i].connectivity.faceIDs[3] - 1;
		int leftCell_4 = facesVector.faces[faceID_4].connectivity.cl[0] - 1;
		int rightCell_4 = facesVector.faces[faceID_4].connectivity.cr[0] - 1;

		// Identify position of adjacent cells based on location of first node
		if (x == cellsVector.cells[i].left && y == cellsVector.cells[i].bottom)			// Bottom left node
		{
			cellsVector.cells[i].bottomCellID = leftCell_1 + rightCell_1 - i;
			cellsVector.cells[i].rightCellID = leftCell_2 + rightCell_2 - i;
			cellsVector.cells[i].topCellID = leftCell_3 + rightCell_3 - i;
			cellsVector.cells[i].leftCellID = leftCell_4 + rightCell_4 - i;
		}
		else if (x == cellsVector.cells[i].right && y == cellsVector.cells[i].bottom)	// Bottom right node
		{
			cellsVector.cells[i].rightCellID = leftCell_1 + rightCell_1 - i;
			cellsVector.cells[i].topCellID = leftCell_2 + rightCell_2 - i;
			cellsVector.cells[i].leftCellID = leftCell_3 + rightCell_3 - i;
			cellsVector.cells[i].bottomCellID = leftCell_4 + rightCell_4 - i;
		}
		else if (x == cellsVector.cells[i].right && y == cellsVector.cells[i].top)		// Top right node
		{
			cellsVector.cells[i].topCellID = leftCell_1 + rightCell_1 - i;
			cellsVector.cells[i].leftCellID = leftCell_2 + rightCell_2 - i;
			cellsVector.cells[i].bottomCellID = leftCell_3 + rightCell_3 - i;
			cellsVector.cells[i].rightCellID = leftCell_4 + rightCell_4 - i;
		}
		else if (x == cellsVector.cells[i].left && y == cellsVector.cells[i].top)		// Top left node
		{
			cellsVector.cells[i].leftCellID = leftCell_1 + rightCell_1 - i;
			cellsVector.cells[i].bottomCellID = leftCell_2 + rightCell_2 - i;
			cellsVector.cells[i].rightCellID = leftCell_3 + rightCell_3 - i;
			cellsVector.cells[i].topCellID = leftCell_4 + rightCell_4 - i;
		}
	}
}


// Destructor
Mesh::~Mesh()
{
}


// Assign particle IDs to a cell
void Mesh::addParticlesToCell(int cellID, int particleID)
{
	cellsVector.cells[cellID].listOfParticles.push_back(particleID);
}
