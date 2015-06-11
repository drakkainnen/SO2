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
	random_device generator;
	mt19937 mt(generator());
	uniform_int_distribution<int> wall(0, 3);
	uniform_int_distribution<int> disForX(Simulation::MIN_X, Simulation::MAX_X);
	uniform_int_distribution<int> disForY(Simulation::MIN_Y, Simulation::MAX_Y);
	auto randX = bind(disForX, mt);
	auto randY = bind(disForY, mt);

	auto randWall = wall(mt);
	Human* human = new Human();
	human->setDirection((Direction)randWall);

	if(randWall == Direction::NORTH)
	{
		human->setPosition(randX(), Simulation::MAX_Y);
	}
	else if(randWall == Direction::SOUTH)
	{
		human->setPosition(randX(), Simulation::MIN_Y);
	}
	else if(randWall == Direction::WEST)
	{
		human->setPosition(Simulation::MAX_X, randY());
	}
	else
	{
		human->setPosition(Simulation::MIN_X, randY());
	}

	return human;
}

void* HumanFabric::run()
{
	while(isStoped() == false)
	{
		process();

		for(auto t : humanThreads)
		{
			void* status;
			pthread_tryjoin_np(t, &status);
			if(1L == (long)status)
			{
				cout << "UCIEKL\n";
				pthread_mutex_lock(&Simulation::humanMutex);

				auto ix = humanPositions.begin();
				while(ix != humanPositions.end())
				{
					if((*ix)->isStoped())
					{
						ix = humanPositions.erase(ix);
					}
					else
					{
						++ix;
					}
				}	
				pthread_mutex_unlock(&Simulation::humanMutex);
			}
		}
		usleep(1000000);
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
	humanThreads.push_back(thread);
	pthread_attr_destroy(&attr);
}
