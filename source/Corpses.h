#ifndef Corpses_CLASS
#define Corpses_CLASS

#include <queue>
#include <utility>


class Corpses
{
	int x;
	int y;
	int percent;

public:
	Corpses(int x, int y);
	std::pair<int, int> getPosition();
	int getPercent();
	void porcessBody();
};

#endif //Corpses_CLASS
