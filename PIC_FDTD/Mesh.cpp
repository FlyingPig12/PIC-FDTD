//! \file
//! \brief Implementation of Mesh class 
//! \author Rahul Kalampattel
//! \date Last updated February 2018

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

	// Find boundaries of each cell and identify adjacent/neighbouring cells,
	// also identify connected nodes
	for (int i = 0; i < numCells; i++)
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

		cellsVector.cells[i].width = cellsVector.cells[i].right - cellsVector.cells[i].left;
		cellsVector.cells[i].height = cellsVector.cells[i].top - cellsVector.cells[i].bottom;

		// Get position of first node
		int nodeID1 = cellsVector.cells[i].connectivity.nodeIDs[0] - 1;
		int nodeID2 = cellsVector.cells[i].connectivity.nodeIDs[1] - 1;
		int nodeID3 = cellsVector.cells[i].connectivity.nodeIDs[2] - 1;
		int nodeID4 = cellsVector.cells[i].connectivity.nodeIDs[3] - 1;
		double x = nodesVector.nodes[nodeID1].geometry.X.element(0, 0);
		double y = nodesVector.nodes[nodeID1].geometry.X.element(1, 0);

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

		// TODO: Reorder if statements so that the internal case is first (most
		// likely to be triggered for a regular mesh, hence can reduce time to
		// identify boundary type)

		// Identify position of adjacent cells based on location of first node,
		// identify connected nodes, and positions of cells and nodes w.r.t.
		// boundary of simulation domain
		if (x == cellsVector.cells[i].left && y == cellsVector.cells[i].bottom)			// Bottom left node
		{
			cellsVector.cells[i].firstNodePosition = "BL";
			cellsVector.cells[i].bottomCellID = leftCell_1 + rightCell_1 - i + 1;
			cellsVector.cells[i].rightCellID = leftCell_2 + rightCell_2 - i + 1;
			cellsVector.cells[i].topCellID = leftCell_3 + rightCell_3 - i + 1;
			cellsVector.cells[i].leftCellID = leftCell_4 + rightCell_4 - i + 1;

			nodesVector.nodes[nodeID1].rightNodeID = nodeID2 + 1;
			nodesVector.nodes[nodeID1].topNodeID = nodeID4 + 1;
			nodesVector.nodes[nodeID2].topNodeID = nodeID3 + 1;
			nodesVector.nodes[nodeID2].leftNodeID = nodeID1 + 1;
			nodesVector.nodes[nodeID3].leftNodeID = nodeID4 + 1;
			nodesVector.nodes[nodeID3].bottomNodeID = nodeID2 + 1;
			nodesVector.nodes[nodeID4].bottomNodeID = nodeID1 + 1;
			nodesVector.nodes[nodeID4].rightNodeID = nodeID3 + 1;

			if (cellsVector.cells[i].leftCellID < 1 && cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "BL";
				nodesVector.nodes[nodeID1].boundaryType = "BL";
				nodesVector.nodes[nodeID2].boundaryType = "B";
				nodesVector.nodes[nodeID4].boundaryType = "L";
			}
			else if (cellsVector.cells[i].leftCellID < 1 && cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "TL";
				nodesVector.nodes[nodeID1].boundaryType = "L";
				nodesVector.nodes[nodeID3].boundaryType = "T";
				nodesVector.nodes[nodeID4].boundaryType = "TL";
			}
			else if (cellsVector.cells[i].rightCellID < 1 && cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "TR";
				nodesVector.nodes[nodeID2].boundaryType = "R";
				nodesVector.nodes[nodeID3].boundaryType = "TR";
				nodesVector.nodes[nodeID4].boundaryType = "T";
			}
			else if (cellsVector.cells[i].rightCellID < 1 && cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "BR";
				nodesVector.nodes[nodeID1].boundaryType = "B";
				nodesVector.nodes[nodeID2].boundaryType = "BR";
				nodesVector.nodes[nodeID3].boundaryType = "R";
			}
			else if (cellsVector.cells[i].leftCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "L";
				nodesVector.nodes[nodeID1].boundaryType = "L";
				nodesVector.nodes[nodeID4].boundaryType = "L";
			}
			else if (cellsVector.cells[i].rightCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "R";
				nodesVector.nodes[nodeID2].boundaryType = "R";
				nodesVector.nodes[nodeID3].boundaryType = "R";
			}
			else if (cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "B";
				nodesVector.nodes[nodeID1].boundaryType = "B";
				nodesVector.nodes[nodeID2].boundaryType = "B";
			}
			else if (cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "T";
				nodesVector.nodes[nodeID3].boundaryType = "T";
				nodesVector.nodes[nodeID4].boundaryType = "T";
			}
			else
			{
				cellsVector.cells[i].boundaryType = "internal";
				nodesVector.nodes[nodeID1].boundaryType = "internal";
				nodesVector.nodes[nodeID2].boundaryType = "internal";
				nodesVector.nodes[nodeID3].boundaryType = "internal";
				nodesVector.nodes[nodeID4].boundaryType = "internal";
			}
		}
		else if (x == cellsVector.cells[i].right && y == cellsVector.cells[i].bottom)	// Bottom right node
		{
			cellsVector.cells[i].firstNodePosition = "BR";
			cellsVector.cells[i].rightCellID = leftCell_1 + rightCell_1 - i + 1;
			cellsVector.cells[i].topCellID = leftCell_2 + rightCell_2 - i + 1;
			cellsVector.cells[i].leftCellID = leftCell_3 + rightCell_3 - i + 1;
			cellsVector.cells[i].bottomCellID = leftCell_4 + rightCell_4 - i + 1;

			nodesVector.nodes[nodeID1].topNodeID = nodeID2 + 1;
			nodesVector.nodes[nodeID1].leftNodeID = nodeID4 + 1;
			nodesVector.nodes[nodeID2].leftNodeID = nodeID3 + 1;
			nodesVector.nodes[nodeID2].bottomNodeID = nodeID1 + 1;
			nodesVector.nodes[nodeID3].bottomNodeID = nodeID4 + 1;
			nodesVector.nodes[nodeID3].rightNodeID = nodeID2 + 1;
			nodesVector.nodes[nodeID4].rightNodeID = nodeID1 + 1;
			nodesVector.nodes[nodeID4].topNodeID = nodeID3 + 1;

			if (cellsVector.cells[i].leftCellID < 1 && cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "BL";
				nodesVector.nodes[nodeID1].boundaryType = "B";
				nodesVector.nodes[nodeID3].boundaryType = "L";
				nodesVector.nodes[nodeID4].boundaryType = "BL";
			}
			else if (cellsVector.cells[i].leftCellID < 1 && cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "TL";
				nodesVector.nodes[nodeID2].boundaryType = "T";
				nodesVector.nodes[nodeID3].boundaryType = "TL";
				nodesVector.nodes[nodeID4].boundaryType = "L";
			}
			else if (cellsVector.cells[i].rightCellID < 1 && cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "TR";
				nodesVector.nodes[nodeID1].boundaryType = "R";
				nodesVector.nodes[nodeID2].boundaryType = "TR";
				nodesVector.nodes[nodeID3].boundaryType = "T";
			}
			else if (cellsVector.cells[i].rightCellID < 1 && cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "BR";
				nodesVector.nodes[nodeID1].boundaryType = "BR";
				nodesVector.nodes[nodeID2].boundaryType = "R";
				nodesVector.nodes[nodeID4].boundaryType = "B";
			}
			else if (cellsVector.cells[i].leftCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "L";
				nodesVector.nodes[nodeID3].boundaryType = "L";
				nodesVector.nodes[nodeID4].boundaryType = "L";
			}
			else if (cellsVector.cells[i].rightCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "R";
				nodesVector.nodes[nodeID1].boundaryType = "R";
				nodesVector.nodes[nodeID2].boundaryType = "R";
			}
			else if (cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "B";
				nodesVector.nodes[nodeID1].boundaryType = "B";
				nodesVector.nodes[nodeID4].boundaryType = "B";
			}
			else if (cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "T";
				nodesVector.nodes[nodeID2].boundaryType = "T";
				nodesVector.nodes[nodeID3].boundaryType = "T";
			}
			else
			{
				cellsVector.cells[i].boundaryType = "internal";
				nodesVector.nodes[nodeID1].boundaryType = "internal";
				nodesVector.nodes[nodeID2].boundaryType = "internal";
				nodesVector.nodes[nodeID3].boundaryType = "internal";
				nodesVector.nodes[nodeID4].boundaryType = "internal";
			}
		}
		else if (x == cellsVector.cells[i].right && y == cellsVector.cells[i].top)		// Top right node
		{
			cellsVector.cells[i].firstNodePosition = "TR";
			cellsVector.cells[i].topCellID = leftCell_1 + rightCell_1 - i + 1;
			cellsVector.cells[i].leftCellID = leftCell_2 + rightCell_2 - i + 1;
			cellsVector.cells[i].bottomCellID = leftCell_3 + rightCell_3 - i + 1;
			cellsVector.cells[i].rightCellID = leftCell_4 + rightCell_4 - i + 1;

			nodesVector.nodes[nodeID1].leftNodeID = nodeID2 + 1;
			nodesVector.nodes[nodeID1].bottomNodeID = nodeID4 + 1;
			nodesVector.nodes[nodeID2].bottomNodeID = nodeID3 + 1;
			nodesVector.nodes[nodeID2].rightNodeID = nodeID1 + 1;
			nodesVector.nodes[nodeID3].rightNodeID = nodeID4 + 1;
			nodesVector.nodes[nodeID3].topNodeID = nodeID2 + 1;
			nodesVector.nodes[nodeID4].topNodeID = nodeID1 + 1;
			nodesVector.nodes[nodeID4].leftNodeID = nodeID3 + 1;

			if (cellsVector.cells[i].leftCellID < 1 && cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "BL";
				nodesVector.nodes[nodeID2].boundaryType = "L";
				nodesVector.nodes[nodeID3].boundaryType = "BL";
				nodesVector.nodes[nodeID4].boundaryType = "B";
			}
			else if (cellsVector.cells[i].leftCellID < 1 && cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "TL";
				nodesVector.nodes[nodeID1].boundaryType = "T";
				nodesVector.nodes[nodeID2].boundaryType = "TL";
				nodesVector.nodes[nodeID3].boundaryType = "L";
			}
			else if (cellsVector.cells[i].rightCellID < 1 && cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "TR";
				nodesVector.nodes[nodeID1].boundaryType = "TR";
				nodesVector.nodes[nodeID2].boundaryType = "T";
				nodesVector.nodes[nodeID4].boundaryType = "R";
			}
			else if (cellsVector.cells[i].rightCellID < 1 && cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "BR";
				nodesVector.nodes[nodeID1].boundaryType = "R";
				nodesVector.nodes[nodeID3].boundaryType = "B";
				nodesVector.nodes[nodeID4].boundaryType = "BR";
			}
			else if (cellsVector.cells[i].leftCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "L";
				nodesVector.nodes[nodeID2].boundaryType = "L";
				nodesVector.nodes[nodeID3].boundaryType = "L";
			}
			else if (cellsVector.cells[i].rightCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "R";
				nodesVector.nodes[nodeID1].boundaryType = "R";
				nodesVector.nodes[nodeID4].boundaryType = "R";
			}
			else if (cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "B";
				nodesVector.nodes[nodeID3].boundaryType = "B";
				nodesVector.nodes[nodeID4].boundaryType = "B";
			}
			else if (cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "T";
				nodesVector.nodes[nodeID1].boundaryType = "T";
				nodesVector.nodes[nodeID2].boundaryType = "T";
			}
			else
			{
				cellsVector.cells[i].boundaryType = "internal";
				nodesVector.nodes[nodeID1].boundaryType = "internal";
				nodesVector.nodes[nodeID2].boundaryType = "internal";
				nodesVector.nodes[nodeID3].boundaryType = "internal";
				nodesVector.nodes[nodeID4].boundaryType = "internal";
			}
		}
		else if (x == cellsVector.cells[i].left && y == cellsVector.cells[i].top)		// Top left node
		{
			cellsVector.cells[i].firstNodePosition = "TL";
			cellsVector.cells[i].leftCellID = leftCell_1 + rightCell_1 - i + 1;
			cellsVector.cells[i].bottomCellID = leftCell_2 + rightCell_2 - i + 1;
			cellsVector.cells[i].rightCellID = leftCell_3 + rightCell_3 - i + 1;
			cellsVector.cells[i].topCellID = leftCell_4 + rightCell_4 - i + 1;

			nodesVector.nodes[nodeID1].bottomNodeID = nodeID2 + 1;
			nodesVector.nodes[nodeID1].rightNodeID = nodeID4 + 1;
			nodesVector.nodes[nodeID2].rightNodeID = nodeID3 + 1;
			nodesVector.nodes[nodeID2].topNodeID = nodeID1 + 1;
			nodesVector.nodes[nodeID3].topNodeID = nodeID4 + 1;
			nodesVector.nodes[nodeID3].leftNodeID = nodeID2 + 1;
			nodesVector.nodes[nodeID4].leftNodeID = nodeID1 + 1;
			nodesVector.nodes[nodeID4].bottomNodeID = nodeID3 + 1;

			if (cellsVector.cells[i].leftCellID < 1 && cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "BL";
				nodesVector.nodes[nodeID1].boundaryType = "L";
				nodesVector.nodes[nodeID2].boundaryType = "BL";
				nodesVector.nodes[nodeID3].boundaryType = "B";
			}
			else if (cellsVector.cells[i].leftCellID < 1 && cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "TL";
				nodesVector.nodes[nodeID1].boundaryType = "TL";
				nodesVector.nodes[nodeID2].boundaryType = "L";
				nodesVector.nodes[nodeID4].boundaryType = "T";
			}
			else if (cellsVector.cells[i].rightCellID < 1 && cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "TR";
				nodesVector.nodes[nodeID1].boundaryType = "T";
				nodesVector.nodes[nodeID3].boundaryType = "R";
				nodesVector.nodes[nodeID4].boundaryType = "TR";
			}
			else if (cellsVector.cells[i].rightCellID < 1 && cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "BR";
				nodesVector.nodes[nodeID2].boundaryType = "B";
				nodesVector.nodes[nodeID3].boundaryType = "BR";
				nodesVector.nodes[nodeID4].boundaryType = "R";
			}
			else if (cellsVector.cells[i].leftCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "L";
				nodesVector.nodes[nodeID1].boundaryType = "L";
				nodesVector.nodes[nodeID2].boundaryType = "L";
			}
			else if (cellsVector.cells[i].rightCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "R";
				nodesVector.nodes[nodeID3].boundaryType = "R";
				nodesVector.nodes[nodeID4].boundaryType = "R";
			}
			else if (cellsVector.cells[i].bottomCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "B";
				nodesVector.nodes[nodeID2].boundaryType = "B";
				nodesVector.nodes[nodeID3].boundaryType = "B";
			}
			else if (cellsVector.cells[i].topCellID < 1)
			{
				cellsVector.cells[i].boundaryType = "T";
				nodesVector.nodes[nodeID1].boundaryType = "T";
				nodesVector.nodes[nodeID4].boundaryType = "T";
			}
			else
			{
				cellsVector.cells[i].boundaryType = "internal";
				nodesVector.nodes[nodeID1].boundaryType = "internal";
				nodesVector.nodes[nodeID2].boundaryType = "internal";
				nodesVector.nodes[nodeID3].boundaryType = "internal";
				nodesVector.nodes[nodeID4].boundaryType = "internal";
			}
		}
	}

	// Function to find opposing cells

	// TODO: for a cell marked to TL, opposing is cell TR
	// Bottom cell for TL is opposed to bottom cell for TR, and so on
	// Similar case when going horizontally, except with TL and BL, i.e. only 
	// need to travel along two edges to work out the opposing sides
	// Iterate through until top/right cell is negative
}


// Destructor
Mesh::~Mesh()
{
}


// Assign particle IDs to a cell
void Mesh::addParticlesToCell(int cellID, int particleID)
{
	cellsVector.cells[cellID - 1].listOfParticles.push_back(particleID);
}


// Remove particle IDs from a cell
void Mesh::removeParticlesFromCell(int cellID, int particleID)
{
	for (int i = 0; i < cellsVector.cells[cellID - 1].listOfParticles.size(); i++)
	{
		if (cellsVector.cells[cellID - 1].listOfParticles[i] == particleID)
		{
			cellsVector.cells[cellID - 1].listOfParticles.erase(cellsVector.cells[cellID - 1].listOfParticles.begin() + i);
		}
	}
}
