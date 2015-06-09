#include "Zombie.h"
#include "Simulation.h"
#include <utility>
#include <random>
#include <pthread.h>
#include <unistd.h>

using namespace std;

Zombie::~Zombie()
{
}

pair<int, int> Zombie::getPosition()
{
	return make_pair(x, y);
}

void Zombie::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Zombie::process()
{
	default_random_engine generator;
	uniform_int_distribution<int> randDirection(0,3);

	pthread_mutex_lock(&Simulation::corpseMutex);
	++x;
	++y;
	pthread_mutex_unlock(&Simulation::corpseMutex);
}

void* Zombie::run()
{
	while(isStoped() == false)
	{
		process();
		usleep(1000000);
		checkAndSuspend();
	}

	pthread_exit((void*)1L);
}
