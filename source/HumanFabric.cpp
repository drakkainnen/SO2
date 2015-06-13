#include "HumanFabric.h"

#include <random>
#include <functional>
#include "Simulation.h"
#include <unistd.h>

using namespace std;

HumanFabric::HumanFabric(std::list<Human*>& humans, std::list<Zombie*>& zombie, std::list<Corpses*>& corpses)
	: Runnable("Human fabric"), humanPositions(humans), zombiePositions(zombie), corpesPositions(corpses)
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

	int x;
	int y;
	if(randWall == Direction::NORTH)
	{
		y = Simulation::MAX_Y;
		x = randX();
	}
	else if(randWall == Direction::SOUTH)
	{
		y = Simulation::MIN_Y;
		x = randX();
	}
	else if(randWall == Direction::WEST)
	{
		y = randY();
		x = Simulation::MAX_X;
	}
	else
	{
		y = randY();
		x = Simulation::MIN_X;
	}
	human->setPosition(x, y);
//	string message = "Human at ("+to_string(x)+", "+to_string(y)+").";
	//human->setMessage(message);

	return human;
}

void* HumanFabric::run()
{
	while(isStoped() == false)
	{
		process();
		usleep(1000000);
		checkAndSuspend();
	}
	pthread_exit((void*)1L);
}

void HumanFabric::process()
{
	Human* h = createHuman();
	pthread_mutex_lock(&Simulation::humanMutex);
	humanPositions.push_back(h);
	createThread(*h);
	pthread_mutex_unlock(&Simulation::humanMutex);
}
