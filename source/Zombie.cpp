#include "Zombie.h"
#include "Human.h"
#include <utility>
#include <random>
#include <pthread.h>
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

	auto newDirection = randDirection(generator);
	//lock
	//if(newDirection == Direction::NORTH && checkNorth())
	//{

	//}
}

void* Zombie::run()
{
	return nullptr;
}
