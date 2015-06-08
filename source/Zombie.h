#ifndef ZOMBIE_CLASS
#define ZOMBIE_CLASS

#include <utility>
#include "Runnable.h"

class Zombie : public Runnable
{
	int x;
	int y;

public:
	~Zombie();
	std::pair<int, int> getPosition();
	void setPosition(int x, int y);
	void process();	
	void* run();
};


#endif //ZOMBIE_CLASS
