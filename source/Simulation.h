#ifndef Simulation_CLASS
#define Simulation_CLASS

#include <pthread.h>
#include <list>

#include "Runnable.h"
#include "ZombieFabric.h"
#include "HumanFabric.h"
#include "Corpses.h"

class Simulation : public Runnable
{
	std::list<Zombie*> zombiePositions;
	std::list<Human*> humanPositions;
	std::list<Corpses*> corpsePositions;

	ZombieFabric* fabricZombie;
	HumanFabric* fabricHuman;

	void prepareLocks();
	void deleteLocks();

	void printHumans();
	void printZombies();
	void printCorpses();

	void createThreads();
	void stopAllThreads();

public:
	static pthread_mutex_t zombieMutex;
	static pthread_mutex_t humanMutex;
	static pthread_mutex_t corpseMutex;

	pthread_t zombieFabricThread;
	pthread_t humanFabricThread;
	pthread_t simulationThread;

	static int MIN_X;
	static int MAX_X;
	static int MIN_Y;
	static int MAX_Y;

	Simulation();
	~Simulation();
	void prepare(const int numberOfThreads);
	void *run();
};

#endif //Simulation_CLASS
