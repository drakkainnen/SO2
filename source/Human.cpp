#include "Human.h"
#include "Simulation.h"
#include <pthread.h>
#include <unistd.h>
#include <random>

using namespace std;

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
}

void* Human::run()
{
	std::random_device generator;
	std::mt19937_64 rand(generator());
	std::uniform_int_distribution<int> dist(500000, 1000000);

	while(isStoped() == false)
	{
		checkAndSuspend();
		usleep(dist(rand));
		process();
		if(x < Simulation::MIN_X || y < Simulation::MIN_Y || x > Simulation::MAX_X	|| y > Simulation::MAX_Y)
		{
			stopThread();
		}
	}
	pthread_exit((void*)1L);
}
