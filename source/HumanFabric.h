#ifndef HumanFabric_CLASS
#define HumanFabric_CLASS

#include "Human.h"
#include "Runnable.h"
#include "Corpses.h"
#include "Zombie.h"
#include <vector>
#include <list>
#include <pthread.h>
#include <random>

class HumanFabric : public Runnable
{
	std::list<Human*>& humanPositions;
	std::list<Zombie*>& zombiePositions;
	std::list<Corpses*>& corpesPositions;

public:
	
	HumanFabric(std::list<Human*>& humans, std::list<Zombie*>& zombie, std::list<Corpses*>& corpses);

	Human* createHuman();
	void* run();
	void process();
};

#endif //HumanFabric_CLASS
