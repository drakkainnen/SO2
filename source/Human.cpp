#include "Human.h"

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
