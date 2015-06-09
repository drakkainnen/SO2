#ifndef HUMAN_CLASS
#define HUMAN_CLASS

#include <utility>
#include "Runnable.h"

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
	bool isEquiped;

	std::pair<int, int> getPosition();
	void setPosition(int x, int y);
	void setDirection(Direction direction);

	void process();
	void* run();

	
	
};

#endif //HUMAN_CLASS
