#include "HumanFabric.h"

#include <random>
#include <functional>

using namespace std;

Human HumanFabric::createHuman()
{
	Human a;
	default_random_engine generator;
	uniform_int_distribution<int> disForX(MIN_X,MAX_X);
	uniform_int_distribution<int> disForY(MIN_X,MAX_X);
	auto randX = bind(disForX, generator);
	auto randY = bind(disForY, generator);

	Human h;
	h.setPosition(randX(), randY());
	h.setDirection(Direction::NORTH);
	
	return move(h);	
}
