#include "Zombie.h"
#include "Simulation.h"
#include <utility>
#include <random>
#include <pthread.h>
#include <unistd.h>

using namespace std;

Zombie::Zombie(std::list<Zombie*>& zombiePositions, std::list<Human*>& humanPositions, std::list<Corpses*>& corpsePositions)
	: Runnable("Zombie"), zombiePositions(zombiePositions), humanPositions(humanPositions), corpsePositions(corpsePositions)
{
}

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
	random_device generator;
	mt19937 mt(generator());
	uniform_int_distribution<int> direction(0, 3);
	
	auto randDirection  = (Direction)direction(mt); 
	int mX = 0;
	int mY = 0;

	//pthread_mutex_lock(&Simulation::humanMutex);
	//Corpses* c = nullptr;
	//for(auto h : humanPositions)
	//{
	//	auto pos = h->getPosition();
	//	if(pos.first == x && pos.second == y)
	//	{
	//		if(h->hasWeapon())
	//		{
	//			setMessage(" killed.");
	//			this->stopThread();	
	//			pthread_mutex_unlock(&Simulation::humanMutex);
	//			return;
	//		}
	//		else
	//		{
	//			h->setMessage(" converted.");
	//			h->stopThread();
	//			c = new Corpses(pos.first, pos.second);
	//		}
	//	}
	//}	
	//pthread_mutex_unlock(&Simulation::humanMutex);

	pthread_mutex_lock(&Simulation::zombieMutex);
	if(randDirection == Direction::NORTH)
	{
		//jezeli idziesz na polnoc a nie mozesz to jednak zawroc
		mY = y-1 < 0 ? 1 : -1;
	}
	else if(randDirection == Direction::SOUTH)
	{
		mY = y+1 > Simulation::MAX_Y ? -1 : 1;
	}
	else if(randDirection == Direction::WEST)
	{
		mX = x-1 < 0 ? 1 : -1;
	}
	else
	{
		mX = x+1 > Simulation::MAX_X ? -1 : 1;
	}

	for(auto z : zombiePositions)
	{
		if(z != this)
		{
			if(z->x == x+mX && z->y == y+mY)
			{
				mX = mY = 0;
				break;
			}
		}
	}
	x += mX;
	y += mY;

	pthread_mutex_unlock(&Simulation::zombieMutex);

	//if(c != nullptr)
	//{
	//	pthread_mutex_lock(&Simulation::corpseMutex);
	//	corpsePositions.push_front(c);		
	//	pthread_mutex_unlock(&Simulation::corpseMutex);
	//}
}

void* Zombie::run()
{
	random_device generator;
	mt19937 mt(generator());
	uniform_int_distribution<int> sleepTime(5000, 1000000);
	while(true)
	{
		pthread_mutex_lock(&this->stopMutex);
		if(isStoped() == true)
		{
			pthread_mutex_unlock(&this->stopMutex);
			break;
		}
		pthread_mutex_unlock(&this->stopMutex);

		process();
		usleep(sleepTime(mt));
		checkAndSuspend();
	}

	pthread_exit((void*)1L);
}
