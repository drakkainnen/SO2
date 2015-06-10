#include "HumanFabric.h"

#include <random>
#include <functional>
#include "Simulation.h"
#include <unistd.h>

using namespace std;

HumanFabric::HumanFabric(std::list<Human*>& humans, std::list<Zombie*>& zombie, std::list<Corpses*>& corpses)
	: humanPositions(humans), zombiePositions(zombie), corpesPositions(corpses)
{
}

Human* HumanFabric::createHuman()
{
	default_random_engine generator;
	uniform_int_distribution<int> disForX(MIN_X,MAX_X);
	uniform_int_distribution<int> disForY(MIN_X,MAX_X);
	auto randX = bind(disForX, generator);
	auto randY = bind(disForY, generator);

	Human* human = new Human();
	human->setPosition(randX(), randY());
	human->setDirection(Direction::SOUTH);
	return human;
}

void* HumanFabric::run()
{
	while(isStoped() == false)
	{
		process();
		usleep(10000000);
		checkAndSuspend();
	}

	for(auto t : humanThreads)
	{
		join(t);
	}

	pthread_exit((void*)1L);
}

void HumanFabric::process()
{
	Human* h = createHuman();
	pthread_mutex_lock(&Simulation::humanMutex);
	humanPositions.push_back(h);
	createHumanThread(*h);
	pthread_mutex_unlock(&Simulation::humanMutex);
}

void HumanFabric::createHumanThread(Human& human)
{	
	pthread_t thread;
	pthread_attr_t attr; //moze detach bylby lepszy
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&thread, &attr, human.starter, (void*)&human);
	humanThreads.push_back(thread);//nie wiem po co mi to na razie
	pthread_attr_destroy(&attr);
}
