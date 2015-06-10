#include "Simulation.h"
#include <ncurses.h>
#include <iostream>
#include <unistd.h>

pthread_mutex_t Simulation::zombieMutex;
pthread_mutex_t Simulation::humanMutex;
pthread_mutex_t Simulation::corpseMutex;

int Simulation::MIN_X = 0;
int Simulation::MAX_X = 0;
int Simulation::MIN_Y = 0;
int Simulation::MAX_Y = 0;

Simulation::Simulation()
	: zombiePositions(), corpsePositions()
{
	fabricZombie = new ZombieFabric(corpsePositions, zombiePositions, humanPositions);
	fabricHuman = new HumanFabric(humanPositions, zombiePositions, corpsePositions);
	corpsePositions.push_back(new Corpses(10,20));
	corpsePositions.push_back(new Corpses(10,20));
	corpsePositions.push_back(new Corpses(10,20));
	corpsePositions.push_back(new Corpses(10,20));
}

Simulation::~Simulation()
{
	for(auto z : corpsePositions)
	{
		delete z;
	}
	for(auto z : zombiePositions)
	{
		delete z;
	}
	delete fabricZombie;
	delete fabricHuman;
}

void Simulation::prepare(const int numberOfThreads)
{
	prepareLocks();

	createThreads();

	this->join(zombieFabricThread);
	this->join(humanFabricThread);
	this->join(simulationThread);
	
	deleteLocks();
	pthread_exit(nullptr);
}

void* Simulation::run()
{
	initscr();

	getmaxyx(stdscr, MAX_Y, MAX_X); 
	MAX_X -= 30;

	char c = 0;
	
	while(c != 'c')
	{
		clear();

		pthread_mutex_lock(&Simulation::humanMutex);
		pthread_mutex_lock(&Simulation::zombieMutex);
		pthread_mutex_lock(&Simulation::corpseMutex);
		printCorpses();
		printHumans();
		printZombies();
		pthread_mutex_unlock(&Simulation::humanMutex);
		pthread_mutex_unlock(&Simulation::zombieMutex);
		pthread_mutex_unlock(&Simulation::corpseMutex);

		refresh();

		noecho();
		timeout(1);
		c = getch();

		if(c == 'p')
		{			
			Runnable::pause();
		}
		else if(c == 'P')
		{			
			Runnable::reasume();
		}
	}
	stopAllThreads();
	endwin();
	pthread_exit((void*)1L);
}

void Simulation::stopAllThreads()
{
	fabricZombie->stopThread();
	for(auto t : zombiePositions)
	{
		t->stopThread();
	}
	fabricHuman->stopThread();
	for(auto t : humanPositions)
	{
		t->stopThread();
	}
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
	//pthread_mutex_lock(&Simulation::humanMutex);
	for(auto human : humanPositions)
	{
		auto pos = human->getPosition();
		mvaddch(pos.second, pos.first, 'H');
	}	
	//pthread_mutex_unlock(&Simulation::humanMutex);
}

void Simulation::printZombies()
{
	//pthread_mutex_lock(&Simulation::zombieMutex);
	for(auto zombie : zombiePositions)
	{
		auto pos = zombie->getPosition();
		mvaddch(pos.second, pos.first, 'Z');
	}	
	//pthread_mutex_unlock(&Simulation::zombieMutex);
}

void Simulation::printCorpses()
{
	//pthread_mutex_lock(&Simulation::corpseMutex);
	for(auto corpse : corpsePositions)
	{
		auto pos = corpse->getPosition();
		mvaddch(pos.second, pos.first, 'X');
	}	
	//pthread_mutex_unlock(&Simulation::corpseMutex);
}

void Simulation::createThreads()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&humanFabricThread, &attr, fabricHuman->starter, (void *)fabricHuman);
	pthread_create(&zombieFabricThread, &attr, fabricZombie->starter, (void *)fabricZombie);
	pthread_create(&simulationThread, &attr, this->starter, (void *)this);

	pthread_attr_destroy(&attr);
}
