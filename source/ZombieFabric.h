#ifndef ZombieFabric_CLASS
#define ZombieFabric_CLASS

#include <vector>
#include "Corpses.h"
#include "Zombie.h"
#include "Runnable.h"

class ZombieFabric : public Runnable
{
	std::vector<Corpses*>& corpsePositions;
	std::vector<Zombie*>& zombiePositions;
	std::vector<pthread_t> threadColection;

	bool createZombie();
	bool createZombieAtRandomPosition();
	void createZombieThread(Zombie& zombie);
		
public:
	ZombieFabric(std::vector<Corpses*>& corpses, std::vector<Zombie*>& zombies);

	void addCorpse(int x, int y);
	void process();

	void* run();

};

#endif //ZombieFabric_CLASS
