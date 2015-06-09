#include <iostream>

#include "Simulation.h"

using namespace std;

int main(int argv, char** args)
{
	Runnable::init();
	Simulation sim;
	sim.prepare(argv);
	Runnable::destroy();
	return 0;
}

