#include "Simulation.h"
#include <ncurses.h>
#include <iostream>
#include <unistd.h>

pthread_mutex_t Simulation::zombieMutex;
pthread_mutex_t Simulation::humanMutex;
pthread_mutex_t Simulation::corpseMutex;

Simulation::Simulation()
	: zombiePositions(), corpsePositions()
{
	fabricZombie = new ZombieFabric(corpsePositions, zombiePositions);
	corpsePositions.push_back(new Corpses(10,20));
	corpsePositions.push_back(new Corpses(11,20));
	corpsePositions.push_back(new Corpses(20,10));
}

void Simulation::prepare(const int numberOfThreads)
{
	prepareLocks();

	createThreads();

	this->join(zombieFabricThread);
	this->join(simulationThread);
	
	deleteLocks();
	pthread_exit(nullptr);
}

void* Simulation::run()
{
	initscr();
	char c = 0;
	
	while(c != 'c')
	{
		printHumans();
		printCorpses();
		printZombies();

		refresh();

		noecho();
		timeout(1);
		c = getch();
	} 
	endwin();
	pthread_exit(new int(1));
}
void Simulation::prepareLocks()
{
	pthread_mutex_init(&Simulation::zombieMutex, NULL);
	pthread_mutex_init(&Simulation::humanMutex, NULL);
	pthread_mutex_init(&Simulation::corpseMutex, NULL);
}

void Simulation::deleteLocks()
{
	pthread_mutex_destroy(&Simulation::zombieMutex);
	pthread_mutex_destroy(&Simulation::humanMutex);
	pthread_mutex_destroy(&Simulation::corpseMutex);
}

void Simulation::printHumans()
{
	pthread_mutex_lock(&Simulation::humanMutex);
	pthread_mutex_unlock(&Simulation::humanMutex);
}

void Simulation::printZombies()
{
	pthread_mutex_lock(&Simulation::zombieMutex);
	pthread_mutex_unlock(&Simulation::zombieMutex);
}

void Simulation::printCorpses()
{
	pthread_mutex_lock(&Simulation::corpseMutex);
	for(auto corpse : corpsePositions)
	{
		auto pos = corpse->getPosition();
		mvaddch(pos.first, pos.second, 'X');
	}	
	pthread_mutex_unlock(&Simulation::corpseMutex);
}

void Simulation::createThreads()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&zombieFabricThread, &attr, fabricZombie->starter, (void *)fabricZombie);
	pthread_create(&simulationThread, &attr, this->starter, (void *)this);

	pthread_attr_destroy(&attr);
}
