//! \file
//! \brief Implementation of Parameters class 
//! \author Rahul Kalampattel
//! \date Last updated April 2018

#include "Parameters.h"

// Default constructor
Parameters::Parameters()
{
}


// Constructor
Parameters::Parameters(std::string filename)
{
	initialTime = std::chrono::system_clock::now();
	std::ifstream inputFile(filename);	// Open input file
	
	char firstCharacter;
	std::string name;
	std::string value;

	if (inputFile.is_open())
	{
		logMessages("Reading inputs", __FILENAME__, __LINE__, 1);

		while (!inputFile.eof())	// Continue until the end of the file is reached
		{
			// Check what the first character in each line is
			firstCharacter = static_cast<char>(inputFile.get());

			// Check for commented or empty lines
			if (firstCharacter == '%' || firstCharacter == '\n')
			{
				inputFile.ignore(256, '\n');
			}
			// Store values in a string vector, names and values are space separated
			else
			{
				inputFile >> name >> value;
				valuesVector.push_back(value);
				inputFile.ignore(256, '\n');
			}
		}
		inputFile.close();
	}
	else
	{
		logMessages("Unable to open input file", __FILE__, __LINE__, 3);
		fileNotOpened = true;
	}
}


// Destructor
Parameters::~Parameters()
{
}


// Assign values to data members 
void Parameters::assignInputs()
{
	logMessages("Printing input parameters", __FILENAME__, __LINE__, 1);
	if (!fileNotOpened)
	{
		int index = 0;


		// Simulation parameters
		try
		{
			timeStep = stod(valuesVector[index]);
			if (timeStep < 0.0)
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid type detected for time step", 3);
		}
		catch (int error)
		{
			logBrief("Time step should be positive", 3);
		}
		logBrief("Time step: " + valuesVector[index], 1);
		index++;


		try
		{
			maximumNumberOfIterations = stoi(valuesVector[index]);
			if (maximumNumberOfIterations < 0)
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for maximum number of iterations", 3);
		}
		catch (int error)
		{
			logBrief("Maximum number of iterations should be positive", 3);
		}
		logBrief("Maximum number of iterations: " + valuesVector[index], 1);
		index++;


		try
		{
			numberOfPatches = stoi(valuesVector[index]);
			if (numberOfPatches < 0)
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for number of patches", 3);
		}
		catch (int error)
		{
			logBrief("Number of patches should be positive", 3);
		}
		logBrief("Number of patches: " + valuesVector[index], 1);
		index++;


		try
		{
			particlesPerCell = stoi(valuesVector[index]);
			if (particlesPerCell < 1)
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for particles per cell", 3);
		}
		catch (int error)
		{
			logBrief("Particles per cell should be positive", 3);
		}
		logBrief("Particles per cell: " + valuesVector[index], 1);
		index++;


		try
		{
			numCellsWithParticles = stoi(valuesVector[index]);
			if (numCellsWithParticles < 0)
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for number of cells with particles", 3);
		}
		catch (int error)
		{
			logBrief("Number of cells with particles should be positive", 3);
		}
		logBrief("Number of cells with particles: " + valuesVector[index], 1);
		index++;


		try
		{
			simulationType = valuesVector[index];
			if (simulationType == "full" || simulationType == "partial" || simulationType == "electron")
			{
			}
			else
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for simulation type", 3);
		}
		catch (int error)
		{
			logBrief("Simulation type should be full, partial, or electron", 3);
		}
		logBrief("Simulation type: " + valuesVector[index], 1);
		index++;


		try
		{
			int value = stoi(valuesVector[index]);
			if (value == 1)
			{
				axisymmetric = true;
			}
			else if (value == 0)
			{
				axisymmetric = false;
			}
			else
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for axisymmetric flag", 3);
		}
		catch (int error)
		{
			logBrief("Axisymmetric flag should be true (1) or false (0)", 3);
		}
		logBrief("Axisymmetric flag: " + valuesVector[index], 1);
		index++;


		try
		{
			int value = stoi(valuesVector[index]);
			if (value == 1)
			{
				twoStream = true;
			}
			else if (value == 0)
			{
				twoStream = false;
			}
			else
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for two-stream flag", 3);
		}
		catch (int error)
		{
			logBrief("Two-stream flag should be true (1) or false (0)", 3);
		}
		logBrief("Two-stream flag: " + valuesVector[index], 1);
		index++;


		// Particle parameters
		try
		{
			particleDistribution = valuesVector[index];
			if (particleDistribution == "uniform" || particleDistribution == "random" || particleDistribution == "precise")
			{
			}
			else
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for particle distribution", 3);
		}
		catch (int error)
		{
			logBrief("Particle distribution should be uniform, random or precise", 3);
		}
		logBrief("Particle distribution: " + valuesVector[index], 1);
		index++;


		try
		{
			initialTemperature = stod(valuesVector[index]);
			if (initialTemperature < 0.0)
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid type detected for initial temperature", 3);
		}
		catch (int error)
		{
			logBrief("Initial temperature should be positive", 3);
		}
		logBrief("Initial temperature: " + valuesVector[index], 1);
		index++;


		try
		{
			std::stringstream inputs(valuesVector[index]);
			std::vector<std::string> outputs;

			while (inputs.good())
			{
				std::string value;
				std::getline(inputs, value, ',');
				outputs.push_back(value);
			}

			for (int i = 0; i < outputs.size(); i++)
			{
				initialPosition.push_back(stod(outputs[i]));
				if (initialPosition[i] < 0.0 || initialPosition[i] > 1.0)
				{
					throw 1;
				}
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for initial position", 3);
		}
		catch (int error)
		{
			logBrief("Initial position should be between 0 and 1", 3);
		}
		logBrief("Initial position: " + valuesVector[index], 1);
		index++;


		try
		{
			std::stringstream inputs(valuesVector[index]);
			std::vector<std::string> outputs;

			while (inputs.good())
			{
				std::string value;
				std::getline(inputs, value, ',');
				outputs.push_back(value);
			}

			for (int i = 0; i < outputs.size(); i++)
			{
				initialVelocity.push_back(stod(outputs[i]));
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for initial velocity", 3);
		}
		logBrief("Initial velocity: " + valuesVector[index], 1);
		index++;


		try
		{
			propellant = valuesVector[index];
			if (propellant != "xenon")
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for propellant", 3);
		}
		catch (int error)
		{
			logBrief("Invalid propellant selected", 3);
		}
		logBrief("Propellant: " + valuesVector[index], 1);
		index++;


		// Field parameters
		try
		{
			std::stringstream inputs(valuesVector[index]);
			std::vector<std::string> outputs;

			while (inputs.good())
			{
				std::string value;
				std::getline(inputs, value, ',');
				outputs.push_back(value);
			}

			for (int i = 0; i < outputs.size(); i++)
			{
				Efield.push_back(stod(outputs[i]));
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for electric field", 3);
		}
		logBrief("Electric field: " + valuesVector[index], 1);
		index++;


		try
		{
			std::stringstream inputs(valuesVector[index]);
			std::vector<std::string> outputs;

			while (inputs.good())
			{
				std::string value;
				std::getline(inputs, value, ',');
				outputs.push_back(value);
			}

			for (int i = 0; i < outputs.size(); i++)
			{
				Bfield.push_back(stod(outputs[i]));
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for magnetic field", 3);
		}
		logBrief("Magnetic field: " + valuesVector[index], 1);
		index++;


		try
		{
			FDTDtimeStep = stod(valuesVector[index]);
			if (FDTDtimeStep < 0.0)
			{
				throw 1;
			}
		}
		catch (const std::exception&)
		{
			logBrief("Invalid argument detected for FDTD time step", 3);
		}
		catch (int error)
		{
			logBrief("FDTD time step should be positive", 3);
		}
		logBrief("FDTD time step: " + valuesVector[index], 1);
		index++;


		// Mesh parameters
		try
		{
			int value = stoi(valuesVector[index]);
			if (value == 1)
			{
				userMesh = true;
			}
			else if (value == 0)
			{
				userMesh = false;
			}
			else
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for user defined mesh flag", 3);
		}
		catch (int error)
		{
			logBrief("User defined mesh flag should be true (1) or false (0)", 3);
		}
		logBrief("User defined mesh flag: " + valuesVector[index], 1);
		index++;


		try
		{
			domainLength = stod(valuesVector[index]);
			if (domainLength < 0.0)
			{
				throw 1;
			}
		}
		catch (const std::exception&)
		{
			logBrief("Invalid argument detected for domain length", 3);
		}
		catch (int error)
		{
			logBrief("Domain length should be positive", 3);
		}
		logBrief("Domain length: " + valuesVector[index], 1);
		index++;


		try
		{
			domainHeight = stod(valuesVector[index]);
			if (domainHeight < 0.0)
			{
				throw 1;
			}
		}
		catch (const std::exception&)
		{
			logBrief("Invalid argument detected for domain height", 3);
		}
		catch (int error)
		{
			logBrief("Domain height should be positive", 3);
		}
		logBrief("Domain height: " + valuesVector[index], 1);
		index++;


		try
		{
			PICspacing = stod(valuesVector[index]);
			if (PICspacing < 0.0)
			{
				throw 1;
			}
		}
		catch (const std::exception&)
		{
			logBrief("Invalid argument detected for PIC grid spacing", 3);
		}
		catch (int error)
		{
			logBrief("PIC grid spacing should be positive", 3);
		}
		logBrief("PIC grid spacing: " + valuesVector[index], 1);
		index++;


		try
		{
			FDTDspacing = stod(valuesVector[index]);
			if (FDTDspacing < 0.0)
			{
				throw 1;
			}
		}
		catch (const std::exception&)
		{
			logBrief("Invalid argument detected for FDTD grid spacing", 3);
		}
		catch (int error)
		{
			logBrief("FDTD grid spacing should be positive", 3);
		}
		logBrief("FDTD grid spacing: " + valuesVector[index], 1);
		index++;


		try
		{
			meshFilePath = valuesVector[index];
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for mesh file path", 3);
		}
		logBrief("Mesh file path: " + valuesVector[index], 1);
		index++;


		try
		{
			meshScalingParameter = stod(valuesVector[index]);
		}
		catch (const std::exception&)
		{
			logBrief("Invalid argument detected for mesh scaling parameter", 3);
		}
		logBrief("Mesh scaling parameter: " + valuesVector[index], 1);
		index++;


		// Solver parameters
		try
		{
			solverType = valuesVector[index];
			if (solverType == "GS" || solverType == "FFT")
			{
			}
			else
			{
				throw 1;
			}
		}
		catch (const std::exception&)
		{
			logBrief("Invalid argument detected for solver type", 3);
		}
		catch (int error)
		{
			logBrief("Solver type should be GS or FFT", 3);
		}
		logBrief("Solver type: " + valuesVector[index], 1);
		index++;


		try
		{
			maxSolverIterations = stoi(valuesVector[index]);
			if (maxSolverIterations < 0)
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for maximum solver iterations", 3);
		}
		catch (int error)
		{
			logBrief("Maximum solver iterations should be positive", 3);
		}
		logBrief("Maximum solver iterations: " + valuesVector[index], 1);
		index++;


		try
		{
			residualTolerance = stod(valuesVector[index]);
			if (residualTolerance < 0.0)
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for residual tolerance", 3);
		}
		catch (int error)
		{
			logBrief("Residual tolerance should be positive", 3);
		}
		logBrief("Residual tolerance: " + valuesVector[index], 1);
		index++;


		try
		{
			SORparameter = stod(valuesVector[index]);
			if (SORparameter < 1.0)
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for successive over-relaxation parameter", 3);
		}
		catch (int error)
		{
			logBrief("Successive over-relaxation parameter should be positive", 3);
		}
		logBrief("Successive over-relaxation parameter: " + valuesVector[index], 1);
		index++;


		try
		{
			leftBCType = valuesVector[index];
			if (leftBCType == "periodic" || leftBCType == "open" || leftBCType == "dirichlet" || leftBCType == "neumann")
			{
			}
			else
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for left boundary condition type", 3);
		}
		catch (int error)
		{
			logBrief("Left boundary condition type should be periodic, open, dirichlet or neumann", 3);
		}
		logBrief("Left boundary condition type: " + valuesVector[index], 1);
		index++;


		try
		{
			leftBCValue = stod(valuesVector[index]);
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for left boundary condition value", 3);
		}
		logBrief("Left boundary condition value: " + valuesVector[index], 1);
		index++;


		try
		{
			rightBCType = valuesVector[index];
			if (rightBCType == "periodic" || rightBCType == "open" || rightBCType == "dirichlet" || rightBCType == "neumann")
			{
			}
			else
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for right boundary condition type", 3);
		}
		catch (int error)
		{
			logBrief("Right boundary condition type should be periodic, open, dirichlet or neumann", 3);
		}
		if ((rightBCType == "periodic" && leftBCType != "periodic") || (rightBCType != "periodic" && leftBCType == "periodic"))
		{
			logBrief("Periodic boundary conditions must be used on left and right", 3);
		}
		logBrief("Right boundary condition type: " + valuesVector[index], 1);
		index++;


		try
		{
			rightBCValue = stod(valuesVector[index]);
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for right boundary condition value", 3);
		}
		logBrief("Right boundary condition value: " + valuesVector[index], 1);
		index++;


		try
		{
			topBCType = valuesVector[index];
			if (topBCType == "periodic" || topBCType == "open" || topBCType == "dirichlet" || topBCType == "neumann")
			{
			}
			else
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for top boundary condition type", 3);
		}
		catch (int error)
		{
			logBrief("Top boundary condition type should be periodic, open, dirichlet or neumann", 3);
		}
		logBrief("Top boundary condition type: " + valuesVector[index], 1);
		index++;


		try
		{
			topBCValue = stod(valuesVector[index]);
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for top boundary condition value", 3);
		}
		logBrief("Top boundary condition value: " + valuesVector[index], 1);
		index++;


		try
		{
			bottomBCType = valuesVector[index];
			if (bottomBCType == "periodic" || bottomBCType == "open" || bottomBCType == "dirichlet" || bottomBCType == "neumann")
			{
			}
			else
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for bottom boundary condition type", 3);
		}
		catch (int error)
		{
			logBrief("Bottom boundary condition type should be periodic, open, dirichlet or neumann", 3);
		}
		if ((bottomBCType == "periodic" && topBCType != "periodic") || (bottomBCType != "periodic" && topBCType == "periodic"))
		{
			logBrief("Periodic boundary conditions must be used on top and bottom", 3);
		}
		logBrief("Bottom boundary condition type: " + valuesVector[index], 1);
		index++;


		try
		{
			bottomBCValue = stod(valuesVector[index]);
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for bottom boundary condition value", 3);
		}
		logBrief("Bottom boundary condition value: " + valuesVector[index], 1);
		index++;


		// Parallelisation parameters
		try
		{
			numThreads = stoi(valuesVector[index]);
			if (numThreads < 1)
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for number of OpenMP threads", 3);
		}
		catch (int error)
		{
			logBrief("Number of threads should be greater than 0", 3);
		}
		logBrief("Number of OpenMP threads: " + valuesVector[index], 1);
		index++;


		// Output parameters
		try
		{
			plotFrequency = stoi(valuesVector[index]);
			if (plotFrequency < 0)
			{
				throw 1;
			}
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for plotting frequency", 3);
		}
		catch (int error)
		{
			logBrief("Plotting frequency should be positive", 3);
		}
		logBrief("Plotting frequency: " + valuesVector[index], 1);
		index++;


		try
		{
			tecplotMesh = valuesVector[index];
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for output mesh file name", 3);
		}
		logBrief("Output mesh file name: " + valuesVector[index], 1);
		index++;


		try
		{
			tecplotParticleSolution = valuesVector[index];
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for particle solution file name", 3);
		}
		logBrief("Particle solution file name: " + valuesVector[index], 1);
		index++;


		try
		{
			tecplotNodeSolution = valuesVector[index];
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for node solution file name", 3);
		}
		logBrief("Node solution file name: " + valuesVector[index], 1);
		index++;


		try
		{
			tecplotGlobalSolution = valuesVector[index];
		}
		catch (std::invalid_argument&)
		{
			logBrief("Invalid argument detected for global solution file name", 3);
		}
		logBrief("Global solution file name: " + valuesVector[index], 1);
		index++;
	}
}


// Process mesh file
void Parameters::generateMesh(std::string type)
{
	double h;
	std::string meshFile;
	if (type == "PIC")
	{
		h = PICspacing;
		meshFile = meshFilePIC;
	}
	else if (type == "FDTD")
	{
		h = FDTDspacing;
		meshFile = meshFileFDTD;
	}

	if (abs((domainLength / h) - round(domainLength / h)) <= 1e-10 &&
		abs((domainHeight / h) - round(domainHeight / h)) <= 1e-10)
	{
		// Number of cells in each direction (length and height)
		int NL = round(domainLength / h);
		int NH = round(domainHeight / h);

		// Number of cells, nodes and faces
		int NCM = NL * NH;
		int NNM = (NL + 1) * (NH + 1);
		int NFM = (NL + 1) * NH + (NH + 1) * NL;

		std::vector<std::vector<double>> nodesVector;
		std::vector<std::vector<int>> cellsVector;
		std::vector<std::vector<int>> facesVector;

		// Create nodes
		int nodeID = 0;
		for (int i = 0; i < NL + 1; i++)
		{
			for (int j = 0; j < NH + 1; j++)
			{
				std::vector<double> node = { h * i,domainHeight - h * j };
				nodesVector.push_back(node);

				// Check for boundary nodes
				if (abs(nodesVector[nodeID][0] - 0.0) <= 1e-10 ||
					abs(nodesVector[nodeID][0] - domainLength) <= 1e-10 ||
					abs(nodesVector[nodeID][1] - 0.0) <= 1e-10 ||
					abs(nodesVector[nodeID][1] - domainHeight) <= 1e-10)
				{
					nodesVector[nodeID].push_back(1.0);
				}
				else
				{
					nodesVector[nodeID].push_back(0.0);
				}

				nodeID++;
			}
		}

		// Create cells and link to nodes
		for (int cellID = 1; cellID < NCM + 1; cellID++)
		{
			int column = floor(cellID / NH);
			int row = cellID % NH;
			if (row == 0)
			{
				column--;
				row = NH;
			}

			int node1 = (NH + 1)*column + row;
			int node2 = (NH + 1)*column + row + 1;
			int node3 = (NH + 1)*(column + 1) + row + 1;
			int node4 = (NH + 1)*(column + 1) + row;

			std::vector<int> cell = { node1,node2,node3,node4,0,0,0,0 };
			cellsVector.push_back(cell);
		}

		// Create faces and link to nodes and cells
		int faceID = 0;
		for (int cellID = 1; cellID < NCM + 1; cellID++)
		{
			int node1 = cellsVector[cellID - 1][0];
			int node2 = cellsVector[cellID - 1][1];
			int node3 = cellsVector[cellID - 1][2];
			int node4 = cellsVector[cellID - 1][3];

			// Left face
			if (nodesVector[node1 - 1][2] == 1.0 &&
				nodesVector[node2 - 1][2] == 1.0)
			{
				faceID++;
				std::vector<int> face = { -1,node1,node2,cellID,0 };
				facesVector.push_back(face);
				cellsVector[cellID - 1][4] = faceID;
			}
			else if (node1 < node2)
			{
				faceID++;
				std::vector<int> face = { 0,node2,node1,cellID - NH,cellID };
				facesVector.push_back(face);
				cellsVector[cellID - 1][4] = faceID;
				cellsVector[cellID - NH - 1][6] = faceID;
			}

			// Bottom face
			if (nodesVector[node2 - 1][2] == 1.0 &&
				nodesVector[node3 - 1][2] == 1.0)
			{
				faceID++;
				std::vector<int> face = { -1,node2,node3,cellID,0 };
				facesVector.push_back(face);
				cellsVector[cellID - 1][5] = faceID;
			}
			else if (node2 < node3)
			{
				faceID++;
				std::vector<int> face = { 0,node2,node3,cellID,cellID + 1 };
				facesVector.push_back(face);
				cellsVector[cellID - 1][5] = faceID;
				cellsVector[cellID][7] = faceID;
			}

			// Right face
			if (nodesVector[node3 - 1][2] == 1.0 &&
				nodesVector[node4 - 1][2] == 1.0)
			{
				faceID++;
				std::vector<int> face = { -1,node3,node4,cellID,0 };
				facesVector.push_back(face);
				cellsVector[cellID - 1][6] = faceID;
			}

			// Top face
			if (nodesVector[node4 - 1][2] == 1.0 &&
				nodesVector[node1 - 1][2] == 1.0)
			{
				faceID++;
				std::vector<int> face = { -1,node4,node1,cellID,0 };
				facesVector.push_back(face);
				cellsVector[cellID - 1][7] = faceID;
			}
		}

		std::ofstream mesh_file_OP2(meshFile + ".op2", std::ios::trunc);

		// Write mesh file in OP2A format
		if (mesh_file_OP2.is_open())
		{
			mesh_file_OP2 << "%*****************************************************" << std::endl;
			mesh_file_OP2 << "%     OP2A Grid File using FORMAT version 1.0         " << std::endl;
			mesh_file_OP2 << "%                                                     " << std::endl;
			mesh_file_OP2 << "%                  Originally developed by Minkwan Kim" << std::endl;
			mesh_file_OP2 << "%=====================================================" << std::endl;
			mesh_file_OP2 << "% [FACE / CELL TYPE identifiers]                      " << std::endl;
			mesh_file_OP2 << "%      Line                   :  3                    " << std::endl;
			mesh_file_OP2 << "%      Triangle (3-nodes)     :  5                    " << std::endl;
			mesh_file_OP2 << "%      Quadrilateral (4-nodes):  9                    " << std::endl;
			mesh_file_OP2 << "%      Tetrahedral (4-nodes)  : 10                    " << std::endl;
			mesh_file_OP2 << "%      Hexahedral (8-nodes)   : 12                    " << std::endl;
			mesh_file_OP2 << "%      Prism (6-nodes)        : 13                    " << std::endl;
			mesh_file_OP2 << "%      Pyramid (5-nodes)      : 14                    " << std::endl;
			mesh_file_OP2 << "%                                                     " << std::endl;
			mesh_file_OP2 << "% [BC TYPE identifiers]                               " << std::endl;
			mesh_file_OP2 << "%      Interior               :  0                    " << std::endl;
			mesh_file_OP2 << "%      Wall                   : 10                    " << std::endl;
			mesh_file_OP2 << "%      Inlet                  : 20                    " << std::endl;
			mesh_file_OP2 << "%      Outlet                 : 30                    " << std::endl;
			mesh_file_OP2 << "%      Freestream (Far-field) : 40                    " << std::endl;
			mesh_file_OP2 << "%      Symmetric              : 50                    " << std::endl;
			mesh_file_OP2 << "%      Axisymmetric           : 60                    " << std::endl;
			mesh_file_OP2 << "%      Anode                  : 70                    " << std::endl;
			mesh_file_OP2 << "%      Cathode                : 80                    " << std::endl;
			mesh_file_OP2 << "%      Dielectric material    : 90                    " << std::endl;
			mesh_file_OP2 << "%                                                     " << std::endl;
			mesh_file_OP2 << "% [DATA Format]                                       " << std::endl;
			mesh_file_OP2 << "%    -Node Data FORMAT                                " << std::endl;
			mesh_file_OP2 << "%     (NODE ID#)  (POSITION DATA: X Y Z)           " << std::endl;
			mesh_file_OP2 << "%    -Face Data FORMAT                                " << std::endl;
			mesh_file_OP2 << "%     (FACE ID#)  (BOUNDARY CONDITION)  (FACE TYPE)  (LIST OF NODE IDs) (LEFT-CELL ID) (RIGHT-CELL ID)" << std::endl;
			mesh_file_OP2 << "%    -Cell Data FORMAT" << std::endl;
			mesh_file_OP2 << "%     (CELL ID#)  (BOUNDARY CONDITION)  (CELL TYPE)  (LIST OF NODE IDs) (LIST OF FACE IDs)" << std::endl;
			mesh_file_OP2 << "%=====================================================" << std::endl;
			mesh_file_OP2 << "%                   Last Format Updated on April/2016 " << std::endl;
			mesh_file_OP2 << "%                                       by M Kim      " << std::endl;
			mesh_file_OP2 << "%*****************************************************" << std::endl << std::endl;

			mesh_file_OP2 << "% OVERALL GRID INFORMATION" << std::endl;
			mesh_file_OP2 << "DIM= " << 2 << std::endl;
			mesh_file_OP2 << "NNM= " << NNM << std::endl;
			mesh_file_OP2 << "NFM= " << NFM << std::endl;
			mesh_file_OP2 << "NCM= " << NCM << std::endl;
			mesh_file_OP2 << std::endl;

			mesh_file_OP2 << "%%%%%%%%%%%" << std::endl;
			mesh_file_OP2 << "NODEDATA= " << NNM << std::endl;
			for (int n = 0; n < NNM; n++)
			{
				mesh_file_OP2 << std::setw(8) << n + 1 << std::setw(25) <<
					std::scientific << std::setprecision(16) <<
					nodesVector[n][0] << std::setw(25) << nodesVector[n][1] <<
					std::setw(25) << 0.0 << std::endl;
			}
			mesh_file_OP2 << std::endl;

			mesh_file_OP2 << "%%%%%%%%%%%%" << std::endl;
			mesh_file_OP2 << "FACEDATA= " << NFM << std::endl;
			for (int f = 0; f < NFM; f++)
			{
				mesh_file_OP2 << std::setw(8) << f + 1 << std::setw(8) <<
					facesVector[f][0] << std::setw(8) << 3;
				for (int i = 1; i <= 4; i++)
					mesh_file_OP2 << std::setw(8) << facesVector[f][i];
				mesh_file_OP2 << std::endl;
			}
			mesh_file_OP2 << std::endl;

			mesh_file_OP2 << "%%%%%%%%%%%" << std::endl;
			mesh_file_OP2 << "CELLDATA= " << NCM << std::endl;
			for (int c = 0; c < NCM; c++)
			{
				mesh_file_OP2 << std::setw(8) << c + 1 << std::setw(8) << 0 <<
					std::setw(8) << 9;
				for (int i = 0; i < 8; i++)
					mesh_file_OP2 << std::setw(8) << cellsVector[c][i];
				mesh_file_OP2 << std::endl;
			}
			mesh_file_OP2 << std::endl;
			mesh_file_OP2.close();
		}
	}
	else
	{
		logBrief("Grid spacing not uniform, please adjust", 3);
	}
}


// Process mesh file
void Parameters::processMesh(std::string type)
{
	logMessages("Extracting mesh data", __FILENAME__, __LINE__, 1);

	if (type == "PIC")
	{
		if (userMesh)
		{
			generateMesh("PIC");
		}
		else
		{
			precessingGridSU2(meshFilePath, meshFilePIC);
		}
		readGridFromFile(meshFilePIC + ".op2", gridinfoPIC, gridgeoPIC);
		processingGrid(gridinfoPIC, gridgeoPIC);
	}
	else if (type == "FDTD")
	{
		generateMesh("FDTD");
		readGridFromFile(meshFileFDTD + ".op2", gridinfoFDTD, gridgeoFDTD);
		processingGrid(gridinfoFDTD, gridgeoFDTD);
	}
}


// Log messages, warnings and errors
void Parameters::logMessages(std::string message, std::string filename, int line, int messageType)
{
	std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
	std::chrono::duration<double> duration;
	duration = currentTime - initialTime;

	if (!firstLog)
	{
		std::ofstream logFile("logFile.txt", std::ios::app);	// Open log file, 'append' write mode

		if (logFile.is_open())
		{	
			if (messageType == 1)
			{
				logFile << std::left << std::setfill('.') << std::setw(45) << "(" +
					filename + ", line " + std::to_string(line) + ")" << message <<
					std::right << std::setw(100 - message.length()) << "Elapsed time: " +
					std::to_string(duration.count()) + " seconds" << std::endl;
			}
			else if (messageType == 2)
			{
				logFile << std::left << std::setfill('.') << std::setw(45) << "(" +
					filename + ", line " + std::to_string(line) + ")" << "## WARNING: " +
					message << std::right << std::setw(100 - message.length()) << 
					"Elapsed time: " + std::to_string(duration.count()) + " seconds" <<
					std::endl;
			}
			else if (messageType == 3)
			{
				logFile << std::left << std::setfill('.') << std::setw(45) << "(" +
					filename + ", line " + std::to_string(line) + ")" << "#### ERROR: " + 
					message << std::right << std::setw(100 - message.length()) << 
					"Elapsed time: " + std::to_string(duration.count()) + " seconds" <<
					std::endl;
				numErrors += 1;
			}
			logFile.close();
		}
		else
		{
			std::cout << "Unable to open log file!!!" << std::endl;
		}
		std::cout << message << std::endl;
	}
	else
	{
		std::ofstream logFile("logFile.txt", std::ios::trunc);	// Open log file, 'truncate' write mode

		if (logFile.is_open())
		{
			char time[26];
			std::time_t clockTime = std::chrono::system_clock::to_time_t(currentTime);
			ctime_s(time, sizeof time, &clockTime);
			logFile << "Simulation start time: " << time << std::endl;
			
			logFile << std::left << std::setfill('.') << std::setw(45) << "(" + 
				filename + ", line " + std::to_string(line) + ")" << message << 
				std::right << std::setw(100 - message.length()) << "Elapsed time: " + 
				std::to_string(duration.count()) + " seconds" << std::endl;
			logFile.close();
		}
		else
		{
			std::cout << "Unable to open log file!!!" << std::endl;
		}
		std::cout << message << std::endl;
		firstLog = false;
	}

}


// Log brief messages
void Parameters::logBrief(std::string message, int messageType)
{
	std::ofstream logFile("logFile.txt", std::ios::app);	// Open log file, 'append' write mode

	if (logFile.is_open())
	{
		if (messageType == 1)
		{
			logFile << std::left << std::setw(45) << " " << message << std::endl;
			logFile.close();
		}
		else if (messageType == 2)
		{
			logFile << std::left << std::setw(45) << " " << "## WARNING: " + message << std::endl;
			logFile.close();
		}
		else if (messageType == 3)
		{
			logFile << std::left << std::setw(45) << " " << "#### ERROR: " + message << std::endl;
			logFile.close();
			numErrors += 1;
		}
	}
	else
	{
		std::cout << "Unable to open log file!!!" << std::endl;
	}
	std::cout << message << std::endl;
}