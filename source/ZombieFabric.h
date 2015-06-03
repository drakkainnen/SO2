#ifndef ZombieFabric_CLASS
#define ZombieFabric_CLASS

#include <queue>
#include "Corpses.h"

class ZombieFabric
{
	std::queue<Corpses> corpseCollection;
	
	bool createZombie();
	bool createZombieAtRandomPosition();
		
public:
	ZombieFabric(int numberOfZombies);
	void addCorpse(int x, int y);
	void processCorpses();

};

#endif //ZombieFabric_CLASS
