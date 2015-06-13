#ifndef HUMAN_CLASS
#define HUMAN_CLASS

#include "Runnable.h"
#include <utility>
#include <sstream>

enum Direction
{
	NORTH,
	EAST,
	WEST,
	SOUTH
};

class Human : public Runnable
{
	int x;
	int y;

	Direction direction;

public:
	Human();
	bool isEquiped;

	std::pair<int, int> getPosition();
	void setPosition(int x, int y);
	void setDirection(Direction direction);
	bool hasWeapon();

	void process();
	void* run();

	//std::stringstream& getEvents();	
	
};

#endif //HUMAN_CLASS
