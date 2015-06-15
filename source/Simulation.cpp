#include "Simulation.h"
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
	: Runnable("Simulation"), zombiePositions(), corpsePositions()
{
	fabricZombie = new ZombieFabric(corpsePositions, zombiePositions, humanPositions);
	fabricHuman = new HumanFabric(humanPositions, zombiePositions, corpsePositions);
	corpsePositions.push_back(new Corpses(12,20));
	corpsePositions.push_back(new Corpses(12,20));
	corpsePositions.push_back(new Corpses(12,20));
	corpsePositions.push_back(new Corpses(12,20));
	Human* h = new Human();
	h->setPosition(0,20);
	h->setDirection(Direction::EAST);
	humanPositions.push_back(h);
	h = new Human();
	h->setPosition(0,19);
	h->setDirection(Direction::EAST);
	humanPositions.push_back(h);
	h = new Human();
	h->setPosition(0,21);
	h->setDirection(Direction::EAST);
	humanPositions.push_back(h);
	h = new Human();
	h->setPosition(0,20);
	h->setDirection(Direction::EAST);
	humanPositions.push_back(h);
	h = new Human();
	h->setPosition(0,19);
	h->setDirection(Direction::EAST);
	humanPositions.push_back(h);
	h = new Human();
	h->setPosition(0,21);
	h->setDirection(Direction::EAST);
	humanPositions.push_back(h);
	for(auto h : humanPositions)
	{
		createThread(*h);
	}
}

Simulation::~Simulation()
{
	delete fabricZombie;
	delete fabricHuman;
	for(auto z : corpsePositions)
	{
		delete z;
	}
	for(auto z : zombiePositions)
	{
		delete z;
	}
	for(auto c : corpsePositions)
	{
		delete c;
	}
}

void Simulation::prepare()
{
	prepareLocks();

	createThreads();

	join(this);
	
	deleteLocks();
	pthread_exit(nullptr);
}

void* Simulation::run()
{
	initscr();
	curs_set(0);
	getmaxyx(stdscr, MAX_Y, MAX_X); 
   	corpseWindow = newwin(MAX_Y/2, 30, 0, MAX_X-30);
	eventsWindow = newwin(MAX_Y/2, 30, MAX_Y/2, MAX_X-30);
	simWindow = newwin(MAX_Y, MAX_X-31, 0, 0);
	
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);


	MAX_X -= 31;
	MAX_Y -= 1;
	MIN_Y = 1;
	MIN_X = 1;

	char c = 0;
	bool pauseFlag = false;
	
	while(c != 'c')
	{
		wclear(simWindow);
		wclear(corpseWindow);
		wclear(eventsWindow);

		pthread_mutex_lock(&Simulation::humanMutex);
		pthread_mutex_lock(&Simulation::zombieMutex);
		pthread_mutex_lock(&Simulation::corpseMutex);
		printCorpses();
		printHumans();
		printZombies();
		pthread_mutex_unlock(&Simulation::corpseMutex);
		pthread_mutex_unlock(&Simulation::zombieMutex);
		pthread_mutex_unlock(&Simulation::humanMutex);
	
		printEvents();
		
		if(pauseFlag == true)
		{
			mvwaddstr(simWindow, MAX_Y/2, MAX_X/2, "PAUSED");
		}

		wrefresh(simWindow);
		wrefresh(corpseWindow);
		wrefresh(eventsWindow);

		noecho();
		timeout(1);
		c = getch();

		if(c == 'p')
		{			
			Runnable::pause();
			pauseFlag = true;
		}
		else if(c == 'P')
		{			
			Runnable::reasume();
			pauseFlag = false;
		}
	}
	endwin();
	stopAllThreads();
	pthread_exit((void*)1L);
}

void Simulation::stopAllThreads()
{
	fabricZombie->stopThread();
	join(fabricZombie);

	fabricHuman->stopThread();
	join(fabricHuman);

	for(auto t : zombiePositions)
	{
		t->stopThread();
		join(t);
	}
	for(auto t : humanPositions)
	{
		t->stopThread();
		join(t);
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
	auto ix = humanPositions.begin();
	while(ix != humanPositions.end())
	{
		auto human = *ix;
		if(tryJoin(human->thread))
		{
			human->setMessage(" closed");
			pushEvent(human->getMessageAndClean());
			ix = humanPositions.erase(ix);
		}
		else// if(human->isStoped() == false)
		{
			auto pos = human->getPosition();
			char c = human->isEquiped ? 'W' : 'H';
			wattrset(simWindow, COLOR_PAIR(3));
			mvwaddch(simWindow, pos.second, pos.first, c);
			pushEvent(human->getMessageAndClean());
			++ix;
		}
	}	
	//pthread_mutex_unlock(&Simulation::humanMutex);
}

void Simulation::pushEvent(std::string message)
{
	if(message != "")
	{
		events.push_front(message);
		if(events.size() > 10)
		{
			events.pop_back();
		}
	}
}

void Simulation::printZombies()
{
	//pthread_mutex_lock(&Simulation::zombieMutex);
	auto ix = zombiePositions.begin();
	while(ix != zombiePositions.end())
	{
		auto zombie = *ix;
		if(tryJoin(zombie->thread))
		{
			zombie->setMessage(" closed");
			pushEvent(zombie->getMessageAndClean());
			ix = zombiePositions.erase(ix);
		}
		else if(zombie->isStoped() == false)
		{
			auto pos = zombie->getPosition();

			wattrset(simWindow, COLOR_PAIR(2));
			mvwaddch(simWindow, pos.second, pos.first, 'Z');
			pushEvent(zombie->getMessageAndClean());

			Corpses* c = nullptr;
			for(auto h : humanPositions)
			{
				auto posH = h->getPosition();
				if(posH.first == pos.first && posH.second == pos.second)
				{
					if(h->hasWeapon())
					{
						zombie->setMessage(" killed.");
						zombie->stopThread();	
						break;
					}
					else if(h->isStoped() == false)
					{
						h->setMessage(" converted.");
						h->stopThread();
						c = new Corpses(posH.first, posH.second);
						corpsePositions.push_front(c);
					}
				}
			}	
			++ix;
		}
	}	
	//pthread_mutex_unlock(&Simulation::zombieMutex);
}

void Simulation::printCorpses()
{
	//pthread_mutex_lock(&Simulation::corpseMutex);
	int coordY = 0;
	mvwprintw(corpseWindow, coordY, 3, "Stan zwlok");
	++coordY;

	wattron(corpseWindow, COLOR_PAIR(1));
	for(auto ix = corpsePositions.rbegin(); ix != corpsePositions.rend(); ++ix)
	{
		auto pos = (*ix)->getPosition();
		auto per = (*ix)->getPercent();
		wattrset(simWindow, COLOR_PAIR(4));
		mvwaddch(simWindow, pos.second, pos.first, 'X');

		if(coordY < 10)
		{
			int percent = 0;
			int i = 0;
			do
			{
				percent = i*10;
				if(percent != per)
				{
					mvwaddch(corpseWindow, coordY, 1+i, '=');
				}
				else
				{
					mvwprintw(corpseWindow, coordY, 1+i, "> %d%%", per);
				}
				++i;
			}
			while(percent != per);
			++coordY;
		}
	}	
	wattroff(corpseWindow, COLOR_PAIR(1));
	//pthread_mutex_unlock(&Simulation::corpseMutex);
}

void Simulation::createThreads()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&fabricHuman->thread, &attr, fabricHuman->starter, (void *)fabricHuman);
	pthread_create(&fabricZombie->thread, &attr, fabricZombie->starter, (void *)fabricZombie);
	pthread_create(&thread, &attr, this->starter, (void *)this);

	pthread_attr_destroy(&attr);
}

void Simulation::printEvents()
{
	int c = 0;
	mvwaddstr(eventsWindow, c, 3, "Zdarzenia");
	wattron(eventsWindow, COLOR_PAIR(1));
	for(auto e : events)	
	{
		mvwaddstr(eventsWindow, 1+c, 3, e.c_str());
		++c;
	}
	wattroff(eventsWindow, COLOR_PAIR(1));
}
