#include "Human.h"
#include "Simulation.h"
#include <pthread.h>
#include <unistd.h>

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
	while(isStoped() == false)
	{
		checkAndSuspend();
		usleep(1000000);
		process();
		if(x < 0 || y < 0 || x > 10 || y > 10)
		{
			stopThread();
		}
	}
	pthread_exit((void*)1L);
}
