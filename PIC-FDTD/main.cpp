//! \file
//! \brief Entry point into the simulation
//! \author Rahul Kalampattel
//! \date Last updated April 2018

#include "Simulation.h"

int main()
{
	// Preprocessing of inputs
	Parameters parametersList("inputs.txt");	
	parametersList.assignInputs();	

	if (parametersList.numErrors == 0)
	{
		parametersList.processMesh("PIC");

		// Commence simulation
		Simulation simulation(&parametersList);
	}


	if (parametersList.numErrors != 0)
	{
		parametersList.logMessages("Program exited UNSUCCESSFULLY", __FILENAME__, __LINE__, 1);
		return -1;
	}
	else
	{
		parametersList.logMessages("Program exited successfully", __FILENAME__, __LINE__, 1);
		return 0;
	}
}