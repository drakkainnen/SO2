#ifndef Simulation_CLASS
#define Simulation_CLASS

#include <pthread.h>

#include "Runnable.h"
#include "ZombieFabric.h"
#include "Corpses.h"

class Simulation : public Runnable
{
	std::vector<Zombie*> zombiePositions;
	//
	//std::vector<Human> humanPositions;

	std::vector<Corpses*> corpsePositions;

	ZombieFabric* fabricZombie;

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

	static pthread_cond_t pause;
	static bool close;

	Simulation();
	~Simulation();
	void prepare(const int numberOfThreads);
	void *run();
};

#endif //Simulation_CLASS
