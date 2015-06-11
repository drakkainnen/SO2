#include <iostream>

#include "Simulation.h"

using namespace std;

int main()
{
	Runnable::init();
	Simulation sim;
	sim.prepare();
	Runnable::destroy();
	return 0;
}

