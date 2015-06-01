#ifndef HUMAN_CLASS
#define HUMAN_CLASS

#include <utility>

enum class Direction
{
	NORTH,
	EAST,
	WEST,
	SOUTH
};

class Human
{
	int x;
	int y;

	Direction direction;	

public:
	bool isEquiped;

	std::pair<int, int> getPosition();
	void setPosition(int x, int y);
	void setDirection(Direction direction);
	
};

#endif //HUMAN_CLASS
