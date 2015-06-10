#ifndef ZOMBIE_CLASS
#define ZOMBIE_CLASS

#include <utility>
#include <list>
#include "Runnable.h"
#include "Human.h"

class Zombie : public Runnable
{
	int x;
	int y;

	std::list<Zombie*>& zombiePositions;
	std::list<Human*>& humanPositions;

public:
	Zombie(std::list<Zombie*>& zombiePositions, std::list<Human*>& humanPositions);
	~Zombie();
	std::pair<int, int> getPosition();
	void setPosition(int x, int y);
	void process();	
	void* run();
};


#endif //ZOMBIE_CLASS
