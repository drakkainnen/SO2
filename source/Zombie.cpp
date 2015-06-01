#include "Zombie.h"
#include <utility>

using namespace std;

pair<int, int> Zombie::getPosition()
{
	return make_pair(x, y);
}

void Zombie::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}
