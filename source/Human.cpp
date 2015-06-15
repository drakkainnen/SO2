#include "Human.h"
#include "Simulation.h"
#include <pthread.h>
#include <unistd.h>
#include <random>

using namespace std;

Human::Human()
	: Runnable("Human")
{

	std::random_device generator;
	std::mt19937_64 rand(generator());
	std::uniform_int_distribution<int> dist(0, 10);
	isEquiped = dist(rand) < 3 ? true : false;
}

pair<int, int> Human::getPosition()
{
	return make_pair(x, y);
}
void Human::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Human::setDirection(Direction direction)
{
	this->direction = direction;
}

void Human::process()
{
	pthread_mutex_lock(&Simulation::humanMutex);	
	if(direction == Direction::NORTH)
	{
		--y;
	}
	else if(direction == Direction::EAST)
	{
		++x;
	}
	else if(direction == Direction::WEST)
	{
		x--;
	}
	else
	{
		++y;
	}
	pthread_mutex_unlock(&Simulation::humanMutex);	

	if(x < Simulation::MIN_X || y < Simulation::MIN_Y || x > Simulation::MAX_X || y > Simulation::MAX_Y)
	{
		stopThread();
	}
}

void* Human::run()
{
	std::random_device generator;
	std::mt19937_64 rand(generator());
	std::uniform_int_distribution<int> dist(500000, 1000000);

	while(true)
	{
		pthread_mutex_lock(&this->stopMutex);
		if(isStoped() == true)
		{
			pthread_mutex_unlock(&this->stopMutex);
			break;
		}
		pthread_mutex_unlock(&this->stopMutex);

		checkAndSuspend();
		usleep(dist(rand));
		process();
	}
	pthread_exit((void*)1L);
}

bool Human::hasWeapon()
{
	bool result = isEquiped;
	isEquiped = false;
	return result;
}
