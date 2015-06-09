#include "ZombieFabric.h"
#include <utility>
#include <random>
#include <unistd.h>
#include "Simulation.h"

ZombieFabric::ZombieFabric(std::vector<Corpses*>& corpses, std::vector<Zombie*>& zombies)
	: corpsePositions(corpses), zombiePositions(zombies)
{
}

bool ZombieFabric::createZombie()
{
	Zombie* zombie = new Zombie();	
	pthread_mutex_lock(&Simulation::zombieMutex);
	zombiePositions.push_back(zombie);
	createZombieThread(*zombie);
	pthread_mutex_unlock(&Simulation::zombieMutex);
	return true;
}

void ZombieFabric::createZombieThread(Zombie& zombie)
{	
	pthread_t thread;
	pthread_attr_t attr; //moze detach bylby lepszy
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&thread, &attr, zombie.starter, (void*)&zombie);
	threadColection.push_back(thread);//nie wiem po co mi to na razie
	pthread_attr_destroy(&attr);
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
	Zombie* zombie = new Zombie();
	zombie->setPosition(x, y);	
	zombiePositions.push_back(zombie);
	createZombieThread(*zombie);
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
		createZombieAtRandomPosition();
		for(auto t : threadColection)
		{
			tryJoin(t);
		}
		usleep(5000000);
	}

	for(auto t : threadColection)
	{
		join(t);
	}
	pthread_exit((void*)1L);
}

