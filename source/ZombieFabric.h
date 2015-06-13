#ifndef ZombieFabric_CLASS
#define ZombieFabric_CLASS

#include <vector>
#include <list>
#include "Corpses.h"
#include "Zombie.h"
#include "Human.h"
#include "Runnable.h"

class ZombieFabric : public Runnable
{
	std::list<Corpses*>& corpsePositions;
	std::list<Zombie*>& zombiePositions;
	std::list<Human*>& humanPositions;

	std::vector<Zombie*> readyZombie;

	bool createZombie(std::pair<int,int> pos);
	bool createZombieAtRandomPosition();
		
public:
	ZombieFabric(std::list<Corpses*>& corpses, std::list<Zombie*>& zombies, std::list<Human*>& humanPositions);

	void addCorpse(int x, int y);
	void process();

	void* run();

};

#endif //ZombieFabric_CLASS
