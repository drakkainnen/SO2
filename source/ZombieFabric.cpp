#include "ZombieFabric.h"
#include <utility>
#include <random>
#include <unistd.h>
#include "Simulation.h"

using namespace std;

ZombieFabric::ZombieFabric(std::list<Corpses*>& corpses, std::list<Zombie*>& zombies, std::list<Human*>& humanPositions)
	: Runnable("Zombie fabric"), corpsePositions(corpses), zombiePositions(zombies), humanPositions(humanPositions)
{
}

bool ZombieFabric::createZombie(pair<int, int> pos)
{
	Zombie* zombie = new Zombie(zombiePositions, humanPositions, corpsePositions);	
	zombie->setPosition(pos.first, pos.second);
	pthread_mutex_lock(&Simulation::zombieMutex);
	zombiePositions.push_back(zombie);
	createThread(*zombie);
	pthread_mutex_unlock(&Simulation::zombieMutex);
	return true;
}

bool ZombieFabric::createZombieAtRandomPosition()
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> disForX(1,6);
	std::uniform_int_distribution<int> disForY(1,6);
	//losujemy pozycje
	int x = disForX(generator);
	int y = disForY(generator);
	bool permit = true;

	pthread_mutex_lock(&Simulation::zombieMutex);
	for(int i = 0; i < 10 ; ++i) //max tries
	{
		for(auto z : zombiePositions)
		{
			auto pos = z->getPosition();
			permit = (pos.first == x && pos.second == y) ? false : true;
		}
		if(permit)
		{
			break;
		}
	}
	Zombie* zombie = new Zombie(zombiePositions, humanPositions, corpsePositions);
	zombie->setPosition(x, y);	
	zombiePositions.push_back(zombie);
	createThread(*zombie);
	pthread_mutex_unlock(&Simulation::zombieMutex);
	return true;
}

void ZombieFabric::addCorpse(int x, int y)
{ 
	Corpses* corpse = new Corpses(x, y);
	pthread_mutex_lock(&Simulation::zombieMutex);
	corpsePositions.push_back(corpse);
	pthread_mutex_unlock(&Simulation::zombieMutex);
}

void* ZombieFabric::run()
{
	while(isStoped() == false)
	{
		Runnable::checkAndSuspend();
		//createZombieAtRandomPosition();
		process();
		usleep(1000000);
	}
	pthread_exit((void*)1L);
}

void ZombieFabric::process()
{
	pthread_mutex_lock(&Simulation::corpseMutex);
	auto ix = corpsePositions.begin();
	while(ix != corpsePositions.end())
	{
		Zombie* z;
		if((*ix)->getPercent() == 100)
		{
			auto pos = (*ix)->getPosition();
			z = new Zombie(zombiePositions, humanPositions, corpsePositions);
			z->setPosition(pos.first, pos.second);
			z->setMessage(" at ("+to_string(pos.first)+", "+to_string(pos.second)+").");
			readyZombie.push_back(z);
			ix = corpsePositions.erase(ix);
		}
		else
		{
			(*ix)->porcessBody();
			++ix;
		}
	}
	pthread_mutex_unlock(&Simulation::corpseMutex);

	while(readyZombie.empty() == false)
	{
		auto z = readyZombie.back();
		readyZombie.pop_back();
		
		pthread_mutex_lock(&Simulation::zombieMutex);
		zombiePositions.push_back(z);
		createThread(*z);
		pthread_mutex_unlock(&Simulation::zombieMutex);
	}
}

