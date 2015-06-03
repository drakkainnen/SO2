#include "ZombieFabric.h"
#include "Zombie.h"
#include <utility>
#include <random>

ZombieFabric::ZombieFabric(int numberOfZombies)
{
	for(int i = 0; i < numberOfZombies; ++i)
	{
		while(createZombie() == false);
	}
}

bool ZombieFabric::createZombie()
{
	//posible place for lock
	auto& corpse = corpseCollection.front();
	Zombie freshZombie;
	auto pos = corpse.getPosition();
	freshZombie.setPosition(pos.first, pos.second);
	
	//tutaj lock. Sprawdzamy pozycje czy nic się tam nie znajduje
	
	return true;
}

bool ZombieFabri::createZombieAtRandomPosition()
{
	//losujemy pozycje
	int x = 0;
	int y = 0;
	//tutaj lock
	Zombie zombie;
	zombie.setPosition(x, y);
	//run thread
	//uwazac moze byc deadlock
	return true;
}

void ZombieFabric::addCorpse(int x, int y)
{ 
	//possible place for lock
	Corpses corpse(x, y);
	corpseCollection.push(corpse);	
}

void processCorpses()
{

}
