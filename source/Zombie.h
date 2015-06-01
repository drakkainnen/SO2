#ifndef ZOMBIE_CLASS
#define ZOMBIE_CLASS

#include <utility>


class Zombie
{
	int x;
	int y;

public:
	std::pair<int, int> getPosition();
	void setPosition(int x, int y);
	
};


#endif //ZOMBIE_CLASS
