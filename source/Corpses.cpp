#include "Corpses.h"
#include <string>

using namespace std;

Corpses::Corpses(int x, int y)
{
	this->x = x;
	this->y = y;
	this->percent = 0;
}
pair<int, int> Corpses::getPosition()
{
	return move(make_pair(x, y));
}

void Corpses::porcessBody()
{
	percent += 10;		
}

int Corpses::getPercent()
{
	return percent;
}
