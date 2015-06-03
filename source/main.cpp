#include <iostream>
#include <string>
#include <ncurses.h>
#include <utility>
#include <sstream>

#include "Human.h"
#include "Zombie.h"

using namespace std;

int main(int argv, char** args)
{
	stringstream ss;
	Zombie z;
	z.setPosition(1, 2);
	auto zombiePos = z.getPosition();
	ss << zombiePos.first << " " << zombiePos.second;

	initscr();
	printw(ss.str().c_str());
	refresh();
	getch();
	endwin();
	return 0;
}

